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

pub struct Replace_Generic_Param_With_Unsatisfied_Const_Bound_329;

impl Mutator for Replace_Generic_Param_With_Unsatisfied_Const_Bound_329 {
    fn name(&self) -> &str {
        "Replace_Generic_Param_With_Unsatisfied_Const_Bound_329"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                let generic_params = collect_generic_params(&func.sig.generics.params);
                if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if has_generic_param(return_type, &generic_params) {
                        let new_param = Ident::new("S", Span::call_site());
                        func.sig.generics.params.push(parse_quote!(S));
                        let mut visitor = ReplaceGenericParamVisitor {
                            generic_params: &generic_params,
                            new_name: new_param,
                            replaced: false,
                        };
                        visitor.visit_type_mut(return_type);
                    }
                }
            } else if let Item::Impl(item_impl) = item {
                let impl_generic_params = collect_generic_params(&item_impl.generics.params);
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(method) = impl_item {
                        let method_generic_params = collect_generic_params(&method.sig.generics.params);
                        let mut combined_params = impl_generic_params.clone();
                        for param in &method_generic_params {
                            combined_params.insert(param.clone());
                        }
                        if let ReturnType::Type(_, return_type) = &mut method.sig.output {
                            if has_generic_param(return_type, &combined_params) {
                                let new_param = Ident::new("S", Span::call_site());
                                item_impl.generics.params.push(parse_quote!(S));
                                let mut visitor = ReplaceGenericParamVisitor {
                                    generic_params: &combined_params,
                                    new_name: new_param,
                                    replaced: false,
                                };
                                visitor.visit_type_mut(return_type);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}

fn collect_generic_params(generics: &Punctuated<GenericParam, Comma>) -> HashSet<Ident> {
    let mut params = HashSet::new();
    for param in generics {
        if let GenericParam::Type(type_param) = param {
            params.insert(type_param.ident.clone());
        }
    }
    params
}

fn has_generic_param(type_: &Type, generic_params: &HashSet<Ident>) -> bool {
    let mut visitor = GenericParamVisitor { generic_params, found: false };
    syn::visit::visit_type(&mut visitor, type_);
    visitor.found
}

struct GenericParamVisitor<'a> {
    generic_params: &'a HashSet<Ident>,
    found: bool,
}

impl<'a> Visit<'_> for GenericParamVisitor<'a> {
    fn visit_ident(&mut self, i: &Ident) {
        if self.found {
            return;
        }
        if self.generic_params.contains(i) {
            self.found = true;
        }
    }
}

struct ReplaceGenericParamVisitor<'a> {
    generic_params: &'a HashSet<Ident>,
    new_name: Ident,
    replaced: bool,
}

impl<'a> VisitMut for ReplaceGenericParamVisitor<'a> {
    fn visit_type_mut(&mut self, i: &mut Type) {
        if self.replaced {
            return;
        }
        if let Type::Path(type_path) = i {
            for segment in &mut type_path.path.segments {
                if self.generic_params.contains(&segment.ident) {
                    segment.ident = self.new_name.clone();
                    self.replaced = true;
                    return;
                }
            }
        }
        syn::visit_mut::visit_type_mut(self, i);
    }
}