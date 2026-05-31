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

pub struct Replace_Specialization_Generic_With_Unconstrained_453;

impl Mutator for Replace_Specialization_Generic_With_Unconstrained_453 {
    fn name(&self) -> &str {
        "Replace_Specialization_Generic_With_Unconstrained_453"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                let mut has_specialization_attr = false;
                for attr in &item_impl.attrs {
                    if attr.path().is_ident("specializes") {
                        has_specialization_attr = true;
                        break;
                    }
                }
                if !has_specialization_attr {
                    continue;
                }
                let generics = &mut item_impl.generics;
                if generics.params.is_empty() {
                    continue;
                }
                let mut used_params = HashSet::new();
                let mut visitor = GenericParamVisitor {
                    used_params: &mut used_params,
                };
                visitor.visit_type(&item_impl.self_ty);
                if let Some((_, trait_path, _)) = &item_impl.trait_ {
                    visitor.visit_path(trait_path);
                }
                let mut candidate_params = Vec::new();
                for (idx, param) in generics.params.iter().enumerate() {
                    if let GenericParam::Type(type_param) = param {
                        if !used_params.contains(&type_param.ident.to_string()) {
                            candidate_params.push(idx);
                        }
                    }
                }
                if !candidate_params.is_empty() {
                    let chosen_idx = *candidate_params.choose(&mut rng).unwrap();
                    if let GenericParam::Type(type_param) = &generics.params[chosen_idx] {
                        let new_name = generate_unique_name(&type_param.ident, &generics.params);
                        let new_param: GenericParam = parse_quote!(#new_name);
                        generics.params[chosen_idx] = new_param;
                    }
                } else {
                    let new_name = generate_unique_name(&Ident::new("U", Span::call_site()), &generics.params);
                    let new_param: GenericParam = parse_quote!(#new_name);
                    generics.params.push(new_param);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets specialization implementations (impl blocks with the `#[specializes]` attribute). It modifies a generic parameter in a specialization impl to be unconstrained relative to the base impl, meaning the parameter appears in the specialization impl's generics list but not in its trait or self type, violating the orphan rules for specialization. The operator selects a generic parameter from the specialization impl (excluding lifetimes) and ensures it is not used in the trait being implemented or the self type after mutation. If all parameters are constrained, the operator may rename an existing parameter to a new name not present in the base impl, but this must avoid introducing undeclared types; instead, it should use a new type parameter name that is declared in the same impl's generics. For example, given a seed program with a base impl `impl<T> Trait for Type<T> {}` and a specialization impl `impl<T> Trait for Type<T> where T: Copy {}`, the operator could mutate the specialization impl to `impl<T, U> Trait for Type<T> where T: Copy {}`, introducing an unconstrained type parameter U. Alternatively, if the specialization impl already has multiple parameters, it could change the usage of one so it becomes unconstrained, e.g., from `impl<T, U> Trait for Type<T> where U: Copy {}` to `impl<T, U> Trait for Type<T> {}` if U is not used in Trait or Type<T>. The operator must preserve valid Rust syntax and avoid introducing ambiguous elements; new type parameters should be added to the impl's generics list with a fresh name not conflicting with existing names in scope. This mutation is likely to trigger bugs in the compiler's specialization checking, particularly around constraint resolution and internal compiler errors (ICEs)."
    }
}

struct GenericParamVisitor<'a> {
    used_params: &'a mut HashSet<String>,
}

impl<'a> Visit<'a> for GenericParamVisitor<'a> {
    fn visit_path(&mut self, path: &'a syn::Path) {
        for segment in &path.segments {
            if let PathArguments::AngleBracketed(args) = &segment.arguments {
                for arg in &args.args {
                    if let GenericArgument::Type(ty) = arg {
                        self.visit_type(ty);
                    }
                }
            }
            self.used_params.insert(segment.ident.to_string());
        }
    }

    fn visit_type_path(&mut self, ty: &'a syn::TypePath) {
        self.visit_path(&ty.path);
    }
}

fn generate_unique_name(base: &Ident, existing_params: &Punctuated<GenericParam, Comma>) -> Ident {
    let mut rng = thread_rng();
    let mut name = base.to_string();
    let existing_names: HashSet<String> = existing_params.iter()
        .filter_map(|p| match p {
            GenericParam::Type(t) => Some(t.ident.to_string()),
            GenericParam::Lifetime(l) => Some(l.lifetime.to_string()),
            GenericParam::Const(c) => Some(c.ident.to_string()),
        })
        .collect();
    while existing_names.contains(&name) {
        let suffix: u32 = rng.gen_range(0..1000);
        name = format!("{}_{}", base, suffix);
    }
    Ident::new(&name, Span::call_site())
}