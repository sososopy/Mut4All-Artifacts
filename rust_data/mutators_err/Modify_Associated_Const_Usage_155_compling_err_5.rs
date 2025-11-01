use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Modify_Associated_Const_Usage_155;

impl Mutator for Modify_Associated_Const_Usage_155 {
    fn name(&self) -> &str {
        "Modify_Associated_Const_Usage_155"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if let Some(last_segment) = path.segments.last() {
                        if let PathArguments::None = last_segment.arguments {
                            for item in &mut item_impl.items {
                                if let syn::ImplItem::Const(impl_const) = item {
                                    let const_ident = &impl_const.ident;
                                    let const_value = &impl_const.expr;
                                    for item in &mut file.items {
                                        if let Item::Struct(item_struct) = item {
                                            if let syn::Fields::Named(fields_named) = &item_struct.fields {
                                                for field in &fields_named.named {
                                                    if let Type::Array(type_array) = &field.ty {
                                                        if let Expr::Path(expr_path) = &type_array.len {
                                                            if expr_path.path.segments.len() == 2 {
                                                                let type_ident = &expr_path.path.segments[0].ident;
                                                                let value_ident = &expr_path.path.segments[1].ident;
                                                                if value_ident == const_ident {
                                                                    let new_expr: Expr = parse_quote! {
                                                                        get_value::<#type_ident>()
                                                                    };
                                                                    type_array.len = new_expr;
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        let new_function: ItemFn = parse_quote! {
            fn get_value<T: SampleTrait>() -> usize {
                T::VALUE
            }
        };
        file.items.push(Item::Fn(new_function));
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets trait implementations with associated constants. It modifies the usage of these constants in types that rely on const expressions, introducing potential inference challenges. The mutation involves replacing direct constant usage with a function call that retrieves the constant, thus creating conditions that test the compiler's ability to handle associated constants in type inference scenarios."
    }
}