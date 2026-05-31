use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemExtern, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Insert_Panic_ProcMacroAttribute_214;

impl Mutator for Insert_Panic_ProcMacroAttribute_214 {
    fn name(&self) -> &str {
        "Insert_Panic_ProcMacroAttribute_214"
    }
    fn mutate(&self, file: &mut syn::File) {
        // Check if extern crate proc_macro exists
        let has_proc_macro = file.items.iter().any(|item| {
            if let syn::Item::Extern(ItemExtern { ref ident, .. }) = item {
                *ident == "proc_macro"
            } else {
                false
            }
        });

        if !has_proc_macro {
            let extern_item: syn::Item = parse_quote! {
                extern crate proc_macro;
            };
            file.items.insert(0, extern_item);
        }

        // Check if panic_macro function exists
        let has_panic_macro = file.items.iter().any(|item| {
            if let syn::Item::Fn(func) = item {
                func.attrs.iter().any(|attr| attr.path().is_ident("proc_macro_attribute")) && func.sig.ident == "panic_macro"
            } else {
                false
            }
        });

        if !has_panic_macro {
            // Find the first eligible item (function or struct)
            let mut first_eligible_index = None;
            for (i, item) in file.items.iter().enumerate() {
                if matches!(item, syn::Item::Fn(_) | syn::Item::Struct(_)) {
                    first_eligible_index = Some(i);
                    break;
                }
            }

            if let Some(index) = first_eligible_index {
                // Insert the panic_macro function at the found index
                let panic_macro_fn: syn::Item = parse_quote! {
                    #[proc_macro_attribute]
                    fn panic_macro(_attr: proc_macro::TokenStream, _item: proc_macro::TokenStream) -> proc_macro::TokenStream {
                        panic!()
                    }
                };
                file.items.insert(index, panic_macro_fn);

                // Apply the attribute to the item that was at the found index (now at index+1)
                if let Some(item) = file.items.get_mut(index + 1) {
                    match item {
                        syn::Item::Fn(func) => {
                            func.attrs.push(parse_quote! { #[panic_macro] });
                        },
                        syn::Item::Struct(struct_item) => {
                            struct_item.attrs.push(parse_quote! { #[panic_macro] });
                        },
                        _ => {}
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}