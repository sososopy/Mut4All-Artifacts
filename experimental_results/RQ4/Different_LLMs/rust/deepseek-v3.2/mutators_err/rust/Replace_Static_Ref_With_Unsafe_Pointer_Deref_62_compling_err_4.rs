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

pub struct Replace_Static_Ref_With_Unsafe_Pointer_Deref_62;

impl Mutator for Replace_Static_Ref_With_Unsafe_Pointer_Deref_62 {
    fn name(&self) -> &str {
        "Replace_Static_Ref_With_Unsafe_Pointer_Deref_62"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Static(item_static) = item {
                if let Expr::Reference(ref_expr) = &*item_static.expr {
                    if let Expr::Path(path_expr) = &*ref_expr.expr {
                        if path_expr.path.segments.len() == 1 {
                            let ident = &path_expr.path.segments[0].ident;
                            let mut_static_items = file.items.iter().filter(|i| {
                                matches!(i, Item::Static(s) if matches!(s.mutability, syn::StaticMutability::Mut(_)) && s.ident == *ident)
                            }).collect::<Vec<_>>();
                            if mut_static_items.len() > 0 {
                                let unsafe_block: Expr = parse_quote! {
                                    unsafe { &*&#ident as *const _ }
                                };
                                item_static.expr = Box::new(unsafe_block);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces a static initializer expression that is a reference to a mutable static with an unsafe block that dereferences a raw pointer derived from that mutable static. This applies when a static item's initializer contains a shared reference (&T) to a mutable static (static mut). The mutation changes the initializer to use an unsafe block that creates a raw pointer to the mutable static (as *const T) and then immediately dereferences it to produce a shared reference. This can expose issues in const evaluation when handling references derived from pointers to mutable statics within static initializers. For example, if the seed code contains `static REF: &T = &MUT_STATIC;` where `MUT_STATIC` is a `static mut`, mutate it to `static REF: &T = unsafe { &*&MUT_STATIC as *const T };`. Ensure that the types and variable names used are exactly those present in the seed program. The mutation should only apply if the reference is directly to the mutable static item itself, not through other expressions. This operator targets the const-eval handling of pointers and mutable statics, which is a common source of ICEs."
    }
}