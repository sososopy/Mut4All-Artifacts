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

pub struct Inherent_Associated_Type_Projection_With_Higher_Ranked_Lifetime_Substitution_419;

impl Mutator for Inherent_Associated_Type_Projection_With_Higher_Ranked_Lifetime_Substitution_419 {
    fn name(&self) -> &str {
        "Inherent_Associated_Type_Projection_With_Higher_Ranked_Lifetime_Substitution_419"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut iat_impls = Vec::new();
        let mut higher_ranked_functions = Vec::new();

        for item in &file.items {
            if let Item::Impl(item_impl) = item {
                for impl_item in &item_impl.items {
                    if let ImplItem::Type(type_item) = impl_item {
                        iat_impls.push((item_impl, type_item));
                    }
                }
            }
            if let Item::Fn(item_fn) = item {
                let mut has_hrtb = false;
                for param in &item_fn.sig.inputs {
                    if let FnArg::Typed(PatType { ty, .. }) = param {
                        let mut visitor = HRTBVisitor { found: false };
                        visitor.visit_type(ty);
                        has_hrtb = has_hrtb || visitor.found;
                    }
                }
                if let ReturnType::Type(_, ty) = &item_fn.sig.output {
                    let mut visitor = HRTBVisitor { found: false };
                    visitor.visit_type(ty);
                    has_hrtb = has_hrtb || visitor.found;
                }
                if has_hrtb {
                    higher_ranked_functions.push(item_fn);
                }
            }
        }

        if iat_impls.is_empty() || higher_ranked_functions.is_empty() {
            return;
        }

        let mut rng = thread_rng();
        let iat_impl = iat_impls.choose(&mut rng).unwrap();
        let target_fn = higher_ranked_functions.choose(&mut rng).unwrap();
        let target_fn_ident = target_fn.sig.ident.clone();

        let mut visitor = TypeReplacer {
            iat_path: Self::build_iat_path(iat_impl.0, iat_impl.1),
            replacement_type: Self::build_replacement_type(iat_impl.0, iat_impl.1),
            replaced: false,
        };

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident == target_fn_ident {
                    visitor.visit_item_fn_mut(item_fn);
                    if visitor.replaced {
                        break;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets inherent associated type projections within higher-ranked trait bound contexts. It identifies impl blocks with associated type definitions and functions with HRTB signatures. When found, it replaces associated type projections with direct types that incorporate bound lifetimes, potentially creating region constraint mismatches. This aims to expose bugs in region solving when handling complex inherent associated type projections under higher-ranked binders, similar to the ICE bug involving ReLateBound and ReErased regions."
    }
}

impl Inherent_Associated_Type_Projection_With_Higher_Ranked_Lifetime_Substitution_419 {
    fn build_iat_path(impl_item: &syn::ItemImpl, type_item: &syn::ImplItemType) -> TypePath {
        let impl_ty = &impl_item.self_ty;
        let assoc_ident = &type_item.ident;

        let mut segments = Punctuated::new();
        if let Type::Path(TypePath { path, .. }) = impl_ty {
            for segment in &path.segments {
                segments.push(segment.clone());
            }
        }
        segments.push(syn::PathSegment {
            ident: assoc_ident.clone(),
            arguments: PathArguments::None,
        });

        TypePath {
            qself: None,
            path: SynPath {
                leading_colon: None,
                segments,
            },
        }
    }

    fn build_replacement_type(impl_item: &syn::ItemImpl, type_item: &syn::ImplItemType) -> Type {
        let mut lifetimes = Vec::new();
        for param in &impl_item.generics.params {
            if let GenericParam::Lifetime(lifetime_param) = param {
                lifetimes.push(lifetime_param.lifetime.clone());
            }
        }

        if let Type::Reference(type_ref) = &type_item.ty {
            if let Some(lifetime) = &type_ref.lifetime {
                if lifetimes.contains(lifetime) {
                    return Type::Reference(TypeReference {
                        and_token: type_ref.and_token,
                        lifetime: Some(Lifetime::new("'b", Span::call_site())),
                        mutability: type_ref.mutability,
                        elem: type_ref.elem.clone(),
                    });
                }
            }
        }

        if !lifetimes.is_empty() {
            let lifetime = lifetimes.choose(&mut thread_rng()).unwrap();
            parse_quote!(&#lifetime ())
        } else {
            parse_quote!(&'static ())
        }
    }
}

struct HRTBVisitor {
    found: bool,
}

impl Visit<'_> for HRTBVisitor {
    fn visit_type(&mut self, ty: &Type) {
        if let Type::TraitObject(trait_obj) = ty {
            for bound in &trait_obj.bounds {
                if let TypeParamBound::Trait(trait_bound) = bound {
                    if let Some(bound_lifetimes) = &trait_bound.lifetimes {
                        if bound_lifetimes.lifetimes.len() > 0 {
                            self.found = true;
                            return;
                        }
                    }
                }
            }
        }
        syn::visit::visit_type(self, ty);
    }
}

struct TypeReplacer {
    iat_path: TypePath,
    replacement_type: Type,
    replaced: bool,
}

impl VisitMut for TypeReplacer {
    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::Path(type_path) = ty {
            if self.paths_equal(type_path, &self.iat_path) {
                *ty = self.replacement_type.clone();
                self.replaced = true;
            }
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }
}

impl TypeReplacer {
    fn paths_equal(&self, a: &TypePath, b: &TypePath) -> bool {
        a.path.segments.len() == b.path.segments.len() &&
        a.path.segments.iter().zip(b.path.segments.iter()).all(|(a_seg, b_seg)| {
            a_seg.ident == b_seg.ident
        })
    }
}