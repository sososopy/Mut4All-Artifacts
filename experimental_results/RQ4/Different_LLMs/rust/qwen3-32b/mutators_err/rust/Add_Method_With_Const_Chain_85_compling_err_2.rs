use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Add_Method_With_Const_Chain_85;

impl Mutator for Add_Method_With_Const_Chain_85 {
    fn name(&self) -> &str {
        "Add_Method_With_Const_Chain_85"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut target_struct: Option<&mut ItemStruct> = None;
        let mut target_ident: Option<Ident> = None;

        // Find a struct with const generics
        for item in &mut file.items {
            if let syn::Item::Struct(s) = item {
                if s.generics.params.iter().any(|p| matches!(p, GenericParam::Const(_))) {
                    target_struct = Some(s);
                    target_ident = Some(s.ident.clone());
                    break;
                }
            }
        }

        if let (Some(s), Some(ident)) = (target_struct, target_ident) {
            // Create or find impl block
            let mut impl_item = None;
            for item in &mut file.items {
                if let syn::Item::Impl(impl_candidate) = item {
                    if impl_candidate.trait_.is_none() {
                        if let Type::Path(type_path) = &*impl_candidate.self_ty {
                            if let Some(last_segment) = type_path.path.segments.last() {
                                if last_segment.ident == ident {
                                    impl_item = Some(item);
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            // Add method to impl block
            if let Some(impl_item) = impl_item {
                if let syn::Item::Impl(impl_item) = impl_item {
                    let method = parse_quote! {
                        fn increment(self) -> #ident<{ N + 1 }> {
                            #ident::<{ N + 1 }>{}
                        }
                    };
                    impl_item.items.push(syn::ImplItem::Fn(method));
                }
            } else {
                // Create new impl block
                let generics = &s.generics;
                let impl_block = parse_quote! {
                    impl #generics #ident #generics {
                        fn increment(self) -> #ident<{ N + 1 }> {
                            #ident::<{ N + 1 }>{}
                        }
                    }
                };
                file.items.push(syn::Item::Impl(impl_block));
            }

            // Modify test function
            for item in &mut file.items {
                if let syn::Item::Fn(func) = item {
                    if func.sig.ident == "test" {
                        let expr = parse_quote! {
                            #ident::<0>.increment::<_>().increment::<_>()
                        };
                        func.block = parse_quote! {
                            {
                                #expr
                            }
                        };
                        break;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}