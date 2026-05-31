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

pub struct Replace_Static_Lifetime_Bound_With_Generic_Lifetime_Parameter_In_Const_Generic_Parameter_Types_144;

impl Mutator for Replace_Static_Lifetime_Bound_With_Generic_Lifetime_Parameter_In_Const_Generic_Parameter_Types_144 {
    fn name(&self) -> &str {
        "Replace_Static_Lifetime_Bound_With_Generic_Lifetime_Parameter_In_Const_Generic_Parameter_Types_144"
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator replaces 'static lifetime bounds with generic lifetime parameters in const generic parameter types."
    }
    fn mutate(&self, file: &mut syn::File) {
        struct MutatorVisitor<'a> {
            lifetime_name: Option<Lifetime>,
            has_lifetime_param: bool,
        }

        impl<'a> MutatorVisitor<'a> {
            fn new() -> Self {
                MutatorVisitor {
                    lifetime_name: None,
                    has_lifetime_param: false,
                }
            }

            fn find_or_create_lifetime_param(&mut self, generics: &mut Generics) -> Lifetime {
                if let Some(ref lt) = self.lifetime_name {
                    return lt.clone();
                }

                let new_lifetime = Lifetime::new("'a", Span::call_site());
                self.lifetime_name = Some(new_lifetime.clone());

                // Check if lifetime parameter already exists
                for param in &generics.params {
                    if let GenericParam::Lifetime(lp) = param {
                        if lp.lifetime == new_lifetime {
                            self.has_lifetime_param = true;
                            return new_lifetime.clone();
                        }
                    }
                }

                // Insert lifetime parameter at the beginning of generic parameters
                generics.params.insert(0, GenericParam::Lifetime(LifetimeParam {
                    attrs: Vec::new(),
                    lifetime: new_lifetime.clone(),
                    colon_token: None,
                    bounds: Punctuated::new(),
                }));
                self.has_lifetime_param = true;
                new_lifetime
            }

            fn replace_static_in_type(&self, ty: &mut Type, new_lifetime: &Lifetime) -> bool {
                match ty {
                    Type::Reference(ref_type) => {
                        if let Some(ref lifetime) = ref_type.lifetime {
                            if lifetime.to_string() == "'static" {
                                ref_type.lifetime = Some(new_lifetime.clone());
                                return true;
                            }
                        }
                        false
                    }
                    Type::Slice(slice_type) => {
                        self.replace_static_in_type(&mut slice_type.elem, new_lifetime)
                    }
                    Type::Array(array_type) => {
                        self.replace_static_in_type(&mut array_type.elem, new_lifetime)
                    }
                    Type::Tuple(tuple_type) => {
                        let mut changed = false;
                        for elem in &mut tuple_type.elems {
                            changed |= self.replace_static_in_type(elem, new_lifetime);
                        }
                        changed
                    }
                    Type::Path(type_path) => {
                        let mut changed = false;
                        for segment in &mut type_path.path.segments {
                            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                for arg in &mut args.args {
                                    if let GenericArgument::Type(arg_type) = arg {
                                        changed |= self.replace_static_in_type(arg_type, new_lifetime);
                                    }
                                }
                            }
                        }
                        changed
                    }
                    _ => false,
                }
            }

            fn process_generic_param(&mut self, param: &mut GenericParam) -> bool {
                if let GenericParam::Const(const_param) = param {
                    let mut changed = false;
                    let new_lifetime = self.lifetime_name.clone().unwrap_or_else(|| Lifetime::new("'a", Span::call_site()));
                    changed |= self.replace_static_in_type(&mut const_param.ty, &new_lifetime);
                    changed
                } else {
                    false
                }
            }

            fn process_where_clause(&mut self, where_clause: &mut Option<WhereClause>, new_lifetime: &Lifetime) {
                if let Some(clause) = where_clause {
                    for predicate in &mut clause.predicates {
                        match predicate {
                            WherePredicate::Type(pred_type) => {
                                for bound in &mut pred_type.bounds {
                                    if let TypeParamBound::Lifetime(lifetime_bound) = bound {
                                        if lifetime_bound.to_string() == "'static" {
                                            *bound = TypeParamBound::Lifetime(new_lifetime.clone());
                                        }
                                    }
                                }
                            }
                            WherePredicate::Lifetime(pred_lifetime) => {
                                if pred_lifetime.lifetime.to_string() == "'static" {
                                    pred_lifetime.lifetime = new_lifetime.clone();
                                }
                                for bound in &mut pred_lifetime.bounds {
                                    if bound.to_string() == "'static" {
                                        *bound = new_lifetime.clone();
                                    }
                                }
                            }
                            _ => {}
                        }
                    }
                }
            }
        }

        impl<'a> VisitMut for MutatorVisitor<'a> {
            fn visit_item_fn_mut(&mut self, item_fn: &mut ItemFn) {
                let mut changed = false;
                let generics = &mut item_fn.sig.generics;
                for param in &mut generics.params {
                    changed |= self.process_generic_param(param);
                }
                if changed {
                    let new_lifetime = self.find_or_create_lifetime_param(generics);
                    self.process_where_clause(&mut generics.where_clause, &new_lifetime);
                    self.has_lifetime_param = true;
                }
            }

            fn visit_item_struct_mut(&mut self, item_struct: &mut ItemStruct) {
                let mut changed = false;
                let generics = &mut item_struct.generics;
                for param in &mut generics.params {
                    changed |= self.process_generic_param(param);
                }
                if changed {
                    let new_lifetime = self.find_or_create_lifetime_param(generics);
                    self.process_where_clause(&mut generics.where_clause, &new_lifetime);
                    self.has_lifetime_param = true;
                }
            }
        }

        let mut visitor = MutatorVisitor::new();
        visitor.visit_file_mut(file);
    }
}