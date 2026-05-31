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

pub struct Replace_Impl_Comparison_Trait_Return_With_Concrete_Type_278;

impl Mutator for Replace_Impl_Comparison_Trait_Return_With_Concrete_Type_278 {
    fn name(&self) -> &str {
        "Replace_Impl_Comparison_Trait_Return_With_Concrete_Type_278"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ImplComparisonTraitVisitor {
            rng: thread_rng(),
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions returning `impl` of standard comparison traits (PartialEq, PartialOrd, Eq, Ord). It replaces the opaque return type with a concrete type derived from the function's generic parameters or associated types. This transformation stresses the compiler's handling of opaque return type substitution, trait resolution, and associated type inference, potentially triggering internal compiler errors when debug assertions are enabled due to mismatched param types."
    }
}

struct ImplComparisonTraitVisitor {
    rng: rand::rngs::ThreadRng,
}

impl ImplComparisonTraitVisitor {
    fn is_comparison_trait(path: &SynPath) -> bool {
        let segments = &path.segments;
        if segments.len() != 1 {
            return false;
        }
        let ident = segments.first().unwrap().ident.to_string();
        matches!(ident.as_str(), "PartialEq" | "PartialOrd" | "Eq" | "Ord")
    }

    fn collect_available_types(&self, generics: &Generics, where_clause: Option<&WhereClause>) -> Vec<Type> {
        let mut types = Vec::new();
        for param in &generics.params {
            match param {
                GenericParam::Type(type_param) => {
                    types.push(Type::Path(TypePath {
                        qself: None,
                        path: SynPath::from(type_param.ident.clone()),
                    }));
                }
                GenericParam::Lifetime(lifetime_param) => {
                    types.push(Type::Reference(syn::TypeReference {
                        and_token: token::And::default(),
                        lifetime: Some(Lifetime::new(&lifetime_param.lifetime.ident.to_string(), Span::call_site())),
                        mutability: None,
                        elem: Box::new(Type::Path(TypePath {
                            qself: None,
                            path: SynPath::from(Ident::new("u8", Span::call_site())),
                        })),
                    }));
                }
                GenericParam::Const(const_param) => {
                    types.push(Type::Path(TypePath {
                        qself: None,
                        path: SynPath::from(const_param.ident.clone()),
                    }));
                }
            }
        }
        if let Some(where_clause) = where_clause {
            for predicate in &where_clause.predicates {
                if let WherePredicate::Type(pred_type) = predicate {
                    if let Type::Path(type_path) = &pred_type.bounded_ty {
                        types.push(Type::Path(type_path.clone()));
                    }
                }
            }
        }
        types
    }

    fn get_concrete_replacement(&mut self, generics: &Generics, where_clause: Option<&WhereClause>) -> Option<Type> {
        let available = self.collect_available_types(generics, where_clause);
        if available.is_empty() {
            None
        } else {
            Some(available.choose(&mut self.rng).unwrap().clone())
        }
    }
}

impl VisitMut for ImplComparisonT1raitVisitor {
    fn visit_item_fn_mut(&mut self, i: &mut ItemFn) {
        if let ReturnType::Type(_, return_type) = &mut i.sig.output {
            if let Type::ImplTrait(type_impl_trait) = &**return_type {
                for bound in &type_impl_trait.bounds {
                    if let TypeParamBound::Trait(trait_bound) = bound {
                        if Self::is_comparison_trait(&trait_bound.path) {
                            if let Some(concrete_type) = self.get_concrete_replacement(&i.sig.generics, i.sig.generics.where_clause.as_ref()) {
                                **return_type = concrete_type;
                                break;
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_item_fn_mut(self, i);
    }

    fn visit_impl_item_fn_mut(&mut self, i: &mut syn::ImplItemFn) {
        if let ReturnType::Type(_, return_type) = &mut i.sig.output {
            if let Type::ImplTrait(type_impl_trait) = &**return_type {
                for bound in &type_impl_trait.bounds {
                    if let TypeParamBound::Trait(trait_bound) = bound {
                        if Self::is_comparison_trait(&trait_bound.path) {
                            if let Some(concrete_type) = self.get_concrete_replacement(&i.sig.generics, i.sig.generics.where_clause.as_ref()) {
                                **return_type = concrete_type;
                                break;
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_impl_item_fn_mut(self, i);
    }
}