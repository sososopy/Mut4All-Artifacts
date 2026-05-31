use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    ExternItem, Extern, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct ExternTypeInStaticInit_74;

impl Mutator for ExternTypeInStaticInit_74 {
    fn name(&self) -> &str {
        "ExternTypeInStaticInit_74"
    }
    fn mutate(&self, file: &mut syn::File) {
        // Check if extern type is already defined
        let extern_type_exists = file.items.iter().any(|item| {
            if let Item::Extern(ex) = item {
                ex.items.iter().any(|item| {
                    if let ExternItem::Ty(ty) = item {
                        ty.ident.as_ref().map_or(false, |id| id == "ExternType")
                    } else {
                        false
                    }
                })
            } else {
                false
            }
        });

        if !extern_type_exists {
            let extern_block = Item::Extern(parse_quote! {
                extern "C" {
                    pub type ExternType;
                }
            });
            file.items.insert(0, extern_block);
        }

        // Find and modify the first suitable struct
        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                if !struct_item.fields.is_empty() {
                    // Modify the first field to ExternType
                    let first_field = struct_item.fields.iter_mut().next().unwrap();
                    first_field.ty = parse_quote!(ExternType);
                    
                    // Create static variable with unsafe transmute
                    let struct_name = &struct_item.ident;
                    let static_item = Item::Static(parse_quote! {
                        pub static EXTERN_STATIC_74: &'static #struct_name = unsafe { 
                            std::mem::transmute(&{ 
                                let x = 42; 
                                x 
                            }) 
                        };
                    });
                    file.items.push(static_item);
                    break;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}