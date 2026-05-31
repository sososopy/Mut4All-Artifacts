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

pub struct Insert_Closure_With_Assoc_Type_Reference_422;

impl Mutator for Insert_Closure_With_Assoc_Type_Reference_422 {
    fn name(&self) -> &str {
        "Insert_Closure_With_Assoc_Type_Reference_422"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut impls_with_assoc = Vec::new();
        for item in &file.items {
            if let Item::Impl(item_impl) = item {
                for impl_item in &item_impl.items {
                    if let ImplItem::Type(assoc_type) = impl_item {
                        impls_with_assoc.push((item_impl, assoc_type));
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::Reference(type_ref) = &**return_type {
                        if type_ref.lifetime.is_some() {
                            for (impl_item, assoc_type) in &impls_with_assoc {
                                let struct_name = &impl_item.self_ty;
                                let assoc_ident = &assoc_type.ident;
                                let mut lifetimes_in_fn = Vec::new();
                                for param in &item_fn.sig.generics.params {
                                    if let GenericParam::Lifetime(lifetime_param) = param {
                                        lifetimes_in_fn.push(&lifetime_param.lifetime);
                                    }
                                }
                                if !lifetimes_in_fn.is_empty() {
                                    let lifetime_to_use = lifetimes_in_fn[0].clone();
                                    let closure_stmt: Stmt = parse_quote! {
                                        let closure: for<'b> fn(&'b i32) -> #struct_name<'b>::#assoc_ident = |x| x;
                                    };
                                    item_fn.block.stmts.insert(0, closure_stmt);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies functions returning a reference with a named lifetime and inserts a closure that uses an inherent associated type reference from an existing impl block. It constructs a function pointer type with a higher-ranked trait bound that matches the associated type, creating a lifetime mismatch scenario to stress borrow checking analyses. This transformation reuses existing types and lifetimes to trigger discrepancies similar to reported compiler bugs."
    }
}