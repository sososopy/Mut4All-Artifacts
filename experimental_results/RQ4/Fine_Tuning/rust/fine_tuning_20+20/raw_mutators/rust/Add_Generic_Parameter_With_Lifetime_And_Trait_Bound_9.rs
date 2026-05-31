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

pub struct Add_Generic_Parameter_With_Lifetime_And_Trait_Bound_9;

impl Mutator for Add_Generic_Parameter_With_Lifetime_And_Trait_Bound_9 {
    fn name(&self) -> &str {
        "Add_Generic_Parameter_With_Lifetime_And_Trait_Bound_9"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.generics.params.is_empty() {
                    let new_generic: GenericParam = parse_quote!(T_1);
                    func.sig.generics.params.push(new_generic);
                    let new_where_clause: WhereClause = parse_quote!(where for<'a> T_1: 'a + Sized);
                    func.sig.generics.where_clause = Some(new_where_clause);
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.generics.params.is_empty() {
                            let new_generic: GenericParam = parse_quote!(T_1);
                            func.sig.generics.params.push(new_generic);
                            let new_where_clause: WhereClause =
                                parse_quote!(where for<'a> T_1: 'a + Sized);
                            func.sig.generics.where_clause = Some(new_where_clause);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions and inherent methods with no existing generic parameters. It adds a new generic type parameter T_1 and constrains it with a higher-ranked trait bound: for<'a> T_1: 'a + Sized. This mutation stresses the compiler’s ability to handle additional generic type parameters combined with high-rank lifetime bounds (for<'a>) and trait constraints (Sized). It is designed to explore lifetime resolution logic, generic bound propagation, and the trait system’s interaction with opaque constraints. This may trigger compiler ICEs, especially in complex generic or dispatch scenarios."
    }
}