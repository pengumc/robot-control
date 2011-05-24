/* Name: requests.h
 * Project: usbtoi2c
 * Author: Christian Starkjohann, edited by Michiel van der Coelen
 * Creation Date: 2011-04-1
 * Tabsize: 4
 * Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary (CommercialLicense.txt)
 * This Revision: $Id: requests.h 692 2008-11-07 15:07:40Z cs $
 */

/* This header is shared between the firmware and the host software. It
 * defines the USB request numbers (and optionally data types) used to
 * communicate between the host and the device.
 */

#ifndef __REQUESTS_H_INCLUDED__
#define __REQUESTS_H_INCLUDED__

#define USB_HIGHEST_RQ 6

#define CUSTOM_RQ_SET_STATUS    1
/* Set the LED status. Control-OUT.
 * The requested status is passed in the "wValue" field of the control
 * transfer. No OUT data is sent. Bit 0 of the low byte of wValue controls
 * the LED.
 */

#define CUSTOM_RQ_GET_DATA    2
/* Get the current LED status. Control-IN.
 * This control transfer involves a 1 byte data phase where the device sends
 * the current status to the host. The status is in bit 0 of the byte.
 */
#define CUSTOM_RQ_SET_DATA 3

#define CUSTOM_RQ_RESET 4

#define CUSTOM_RQ_LOAD_POS_FROM_I2C 5

#define CUSTOM_RQ_GET_POS 6

#endif /* __REQUESTS_H_INCLUDED__ */
