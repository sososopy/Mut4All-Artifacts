use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprClosure, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Insert_Const_Generic_In_Closure_ParamList_115;

impl Mutator for Insert_Const_Generic_In_Closure_ParamList_115 {
    fn name(&self) -> &str {
        "Insert_Const_Generic_In_Closure_ParamList_115"
    }
    fn mutate(&self, file: &mut syn::File) {
        // Collect all types in the file
        let mut type_collector = TypeCollector::new();
        type_collector.visit_file(file);
        let types = type_collector.types;

        // Collect all closures in the file
        let mut closure_collector = ClosureCollector::new();
        closure_collector.visit_file(file);
        let closures = closure_collector.closures;

        // Process each closure
        for closure in closures {
            if let Some(ty) = select_random_type(&types) {
                // Create the const generic parameter
                let const_param = GenericParam::Const(ConstGenericParam {
                    const_token: Default::default(),
                    ident: Ident::new("_", Span::call_site()),
                    colon_token: Some(Default::default()),
                    ty: Box::new(ty.clone()),
                    eq_token: None,
                    default: None,
                });
                let binder = ClosureLifetimeBinder::For {
                    for_token: Default::default(),
                    generic_params: Punctuated::from_iter(vec![const_param]),
                };
                closure.fn_decl.binder = Some(binder);
            }
        }

        // Add the feature attribute if not present
        if !has_closure_lifetime_binder_feature(file) {
            let feature = parse_quote!(#![feature(closure_lifetime_binder)]);
            file.attrs.insert(0, feature);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts a const generic parameter into every closure's parameter list, replacing the existing parameter list with a `for<const _: T>` clause where T is a type from the seed program. This transformation stresses the compiler's handling of const generics in closures, potentially exposing bugs in closure type inference or generic parameter resolution."
    }
}

// Helper structs and functions
struct TypeCollector {
    types: Vec<Type>,
}

impl TypeCollector {
    fn new() -> Self {
        Self { types: Vec::new() }
    }
}

impl<'ast> Visit for TypeCollector {
    fn visit_type(&mut self, t: &Type) {
        self.types.push(t.clone());
        syn::visit::visit_type(self, t);
    }
}

struct ClosureCollector<'a> {
    closures: Vec<&'a mut ExprClosure>,
}

impl<'a> ClosureCollector<'a> {
    fn new() -> Self {
        Self { closures: Vec::new() }
    }
}

impl<'a> VisitMut for ClosureCollector<'a> {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Closure(closure) = expr {
            self.closures.push(closure);
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}

fn select_random_type(types: &[Type]) -> Option<&Type> {
    if types.is_empty() {
        return None;
    }
    let mut rng = thread_rng();
    types.choose(&mut rng)
}

fn has_closure_lifetime_binder_feature(file: &File) -> bool {
    for attr in &file.attrs {
        if let syn::Meta::NameValue(meta) = &attr.meta {
            if meta.path.is_ident("feature") {
                if let syn::Expr::Lit(expr_lit) = &*meta.value {
                    if let syn::Lit::Str(lit_str) = &expr_lit.lit {
                        if lit_str.value() == "closure_lifetime_binder" {
                            return true;
                        }
                    }
                }
            }
        }
    }
    false
}