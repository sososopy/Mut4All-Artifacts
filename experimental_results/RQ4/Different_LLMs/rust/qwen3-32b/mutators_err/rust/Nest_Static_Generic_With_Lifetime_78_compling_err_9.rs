use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
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

pub struct Nest_Static_Generic_With_Lifetime_78;

impl Mutator for Nest_Static_Generic_With_Lifetime_78 {
    fn name(&self) -> &str {
        "Nest_Static_Generic_With_Lifetime_78"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut static_indices: Vec<usize> = file.items.iter()
            .enumerate()
            .filter_map(|(i, item)| if let syn::Item::Static(_) = item { Some(i) } else { None })
            .collect();

        for &index in static_indices.iter().rev() {
            let item = &mut file.items[index];
            if let syn::Item::Static(static_item) = item {
                let b_name = format_ident!("B_{}", index);
                let a_name = format_ident!("A_{}", index);

                let b_struct = parse_quote! {
                    struct #b_name<T: 'static> {
                        inner: T,
                    }
                };

                let a_struct = parse_quote! {
                    struct #a_name<'a, T: 'static> {
                        inner: &'a #b_name<T>,
                    }
                };

                file.items.insert(index, syn::Item::Struct(b_struct));
                file.items.insert(index + 1, syn::Item::Struct(a_struct));

                let original_type = &static_item.ty;
                let new_type = parse_quote! {
                    #a_name<'static, #original_type>
                };
                static_item.ty = new_type;

                if let Some(expr_box) = std::boxed::Box::take(static_item.expr) {
                    let expr = *expr_box;
                    let new_expr = parse_quote! {
                        #a_name {
                            inner: &#b_name { inner: #expr },
                        }
                    };
                    static_item.expr = Some(Box::new(new_expr));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds nested generic layers to static variable declarations by wrapping their types in structs with 'static lifetime bounds. It introduces an additional reference level in the initializer, increasing nesting depth and testing the compiler's const evaluation in deeply nested statics."
    }
}