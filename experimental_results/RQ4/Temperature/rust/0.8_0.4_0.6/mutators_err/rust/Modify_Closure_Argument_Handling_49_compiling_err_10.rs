use proc_macro2::{Span, Ident};
use quote::quote;
use syn::{parse_quote, spanned::Spanned, token, punctuated::Punctuated, *};
use syn::visit_mut::VisitMut;
use std::iter::FromIterator;

use crate::mutator::Mutator;

pub struct Modify_Closure_Argument_Handling_49;

impl Mutator for Modify_Closure_Argument_Handling_49 {
    fn name(&self) -> &str {
        "Modify_Closure_Argument_Handling_49"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let Some((_, input)) = item_fn.sig.inputs.iter_mut().enumerate().find(|(_, arg)| {
                    if let FnArg::Typed(pat_type) = arg {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if let Some(segment) = type_path.path.segments.last() {
                                return segment.ident == "FnMut";
                            }
                        }
                    }
                    false
                }) {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if let Some(segment) = type_path.path.segments.last_mut() {
                                if let PathArguments::AngleBracketed(angle_bracketed) = &mut segment.arguments {
                                    if let Some(GenericArgument::Type(Type::Tuple(tuple))) = angle_bracketed.args.first() {
                                        let new_args: Punctuated<FnArg, token::Comma> = tuple.elems.iter().enumerate().map(|(i, ty)| {
                                            let arg_ident = Ident::new(&format!("arg{}", i), Span::call_site());
                                            parse_quote!(#arg_ident: #ty)
                                        }).collect::<Punctuated<_, _>>();

                                        pat_type.ty = Box::new(Type::Tuple(TypeTuple {
                                            paren_token: token::Paren::default(),
                                            elems: tuple.elems.clone(),
                                        }));

                                        item_fn.sig.inputs = new_args;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator transforms closures that accept a single tuple argument into closures that accept multiple individual arguments. This alteration tests the compiler's handling of closure argument transformations, potentially revealing issues with tuple unpacking and argument passing in closure contexts."
    }
}