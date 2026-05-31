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

pub struct Specialization_Associated_Type_Substitution_Swap_451;

impl Mutator for Specialization_Associated_Type_Substitution_Swap_451 {
    fn name(&self) -> &str {
        "Specialization_Associated_Type_Substitution_Swap_451"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut available_containers = Vec::new();
        let mut visitor = ContainerCollector {
            containers: &mut available_containers,
        };
        visitor.visit_file(file);

        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if item_impl.trait_.is_none() {
                    continue;
                }
                if !has_specialization_default(item_impl) {
                    continue;
                }
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Type(assoc_type) = impl_item {
                        if let Type::Path(type_path) = &*assoc_type.ty {
                            if let Some(self_segment) = find_self_in_path(&type_path.path) {
                                let trait_generics = &item_impl.generics;
                                let mut candidate_types = Vec::new();
                                for param in &trait_generics.params {
                                    if let GenericParam::Type(type_param) = param {
                                        candidate_types.push(type_param.ident.clone());
                                    }
                                }
                                if !candidate_types.is_empty() {
                                    let chosen = candidate_types.choose(&mut rng);
                                    if let Some(chosen_ident) = chosen {
                                        let new_path = replace_self_with_ident(&type_path.path, &self_segment, chosen_ident);
                                        assoc_type.ty = Box::new(Type::Path(TypePath {
                                            qself: None,
                                            path: new_path,
                                        }));
                                    }
                                } else if !available_containers.is_empty() {
                                    let container = available_containers.choose(&mut rng).unwrap();
                                    let new_path = wrap_self_in_container(&type_path.path, &self_segment, container);
                                    assoc_type.ty = Box::new(Type::Path(TypePath {
                                        qself: None,
                                        path: new_path,
                                    }));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait implementations with specialization that define default associated types referencing Self. It swaps Self with another trait type parameter if available, or wraps Self in an existing generic container (e.g., Option<Self>). This transformation tests the compiler's handling of specialization, associated type substitution, and type parameter resolution, potentially triggering type inference errors or ICEs in the specialization system."
    }
}

fn has_specialization_default(item_impl: &ItemImpl) -> bool {
    for attr in &item_impl.attrs {
        if attr.path().is_ident("default") {
            return true;
        }
    }
    false
}

fn find_self_in_path(path: &SynPath) -> Option<usize> {
    for (i, segment) in path.segments.iter().enumerate() {
        if segment.ident == "Self" {
            return Some(i);
        }
        if let PathArguments::AngleBracketed(args) = &segment.arguments {
            for arg in &args.args {
                if let GenericArgument::Type(Type::Path(type_path)) = arg {
                    if let Some(_) = find_self_in_path(&type_path.path) {
                        return Some(i);
                    }
                }
            }
        }
    }
    None
}

fn replace_self_with_ident(path: &SynPath, self_index: &usize, ident: &Ident) -> SynPath {
    let mut new_path = path.clone();
    let segment = &mut new_path.segments[*self_index];
    segment.ident = ident.clone();
    if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
        for arg in &mut args.args {
            if let GenericArgument::Type(Type::Path(type_path)) = arg {
                if let Some(sub_index) = find_self_in_path(&type_path.path) {
                    let new_sub_path = replace_self_with_ident(&type_path.path, &sub_index, ident);
                    *arg = GenericArgument::Type(Type::Path(TypePath {
                        qself: None,
                        path: new_sub_path,
                    }));
                }
            }
        }
    }
    new_path
}

fn wrap_self_in_container(path: &SynPath, self_index: &usize, container: &Ident) -> SynPath {
    let mut new_path = path.clone();
    let segment = &mut new_path.segments[*self_index];
    let container_path = SynPath {
        leading_colon: None,
        segments: Punctuated::from_iter(vec![
            PathSegment {
                ident: container.clone(),
                arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                    colon2_token: None,
                    lt_token: token::Lt::default(),
                    args: Punctuated::from_iter(vec![GenericArgument::Type(Type::Path(TypePath {
                        qself: None,
                        path: SynPath::from(Ident::new("Self", Span::call_site())),
                    }))]),
                    gt_token: token::Gt::default(),
                }),
            },
        ]),
    };
    segment.ident = container.clone();
    segment.arguments = PathArguments::AngleBracketed(AngleBracketedGenericArguments {
        colon2_token: None,
        lt_token: token::Lt::default(),
        args: Punctuated::from_iter(vec![GenericArgument::Type(Type::Path(TypePath {
            qself: None,
            path: container_path,
        }))]),
        gt_token: token::Gt::default(),
    });
    new_path
}

struct ContainerCollector<'a> {
    containers: &'a mut Vec<Ident>,
}

impl<'a> Visit<'a> for ContainerCollector<'a> {
    fn visit_item(&mut self, node: &'a Item) {
        if let Item::Struct(item_struct) = node {
            self.containers.push(item_struct.ident.clone());
        }
        if let Item::Enum(item_enum) = node {
            self.containers.push(item_enum.ident.clone());
        }
        if let Item::Union(item_union) = node {
            self.containers.push(item_union.ident.clone());
        }
        syn::visit::visit_item(self, node);
    }
    fn visit_path(&mut self, node: &'a SynPath) {
        if node.segments.len() == 1 {
            let ident = &node.segments[0].ident;
            if ident == "Option" || ident == "Vec" || ident == "Box" || ident == "Result" {
                if !self.containers.contains(ident) {
                    self.containers.push(ident.clone());
                }
            }
        }
        syn::visit::visit_path(self, node);
    }
}