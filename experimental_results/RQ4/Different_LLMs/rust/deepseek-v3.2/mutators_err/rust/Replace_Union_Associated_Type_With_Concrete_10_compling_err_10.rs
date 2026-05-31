use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemUnion, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_Union_Associated_Type_With_Concrete_10;

impl Mutator for Replace_Union_Associated_Type_With_Concrete_10 {
    fn name(&self) -> &str {
        "Replace_Union_Associated_Type_With_Concrete_10"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = UnionAssociatedTypeVisitor {
            trait_impls: Vec::new(),
        };
        visitor.visit_file_mut(file);
        
        let mut items_to_insert = Vec::new();
        let mut union_items = Vec::new();
        
        for item in &mut file.items {
            if let Item::Union(item_union) = item {
                union_items.push(item_union.clone());
            }
        }
        
        for item_union in union_items {
            if item_union.generics.params.is_empty() {
                continue;
            }
            
            for field in &item_union.fields.named {
                if let Type::Path(type_path) = &field.ty {
                    let struct_item = find_struct_by_type(file, &type_path.path);
                    if let Some(struct_item) = struct_item {
                        let mut new_fields = Vec::new();
                        let mut changed = false;
                        
                        for struct_field in &struct_item.fields {
                            let mut field_ty = struct_field.ty.clone();
                            if let Type::Path(ref mut path_ty) = field_1ty {
                                if let Some((trait_name, assoc_name, generic_param)) = extract_associated_projection(path_ty) {
                                    if let Some(concrete_type) = find_concrete_type_for_assoc(
                                        &visitor.trait_impls,
                                        &trait_name,
                                        &assoc_name,
                                        &generic_param,
                                        &item_union
                                    ) {
                                        *path_ty = concrete_type;
                                        changed = true;
                                    } else {
                                        let dummy_type: TypePath = parse_quote!(u32);
                                        *path_ty = dummy_type;
                                        changed = true;
                                    }
                                }
                            }
                            new_fields.push(syn::Field {
                                attrs: struct_field.attrs.clone(),
                                vis: struct_field.vis.clone(),
                                mutability: syn::FieldMutability::None,
                                ident: struct_field.ident.clone(),
                                colon_token: struct_field.colon_token,
                                ty: field_ty,
                            });
                        }
                        
                        if changed {
                            let new_struct_name = Ident::new(
                                &format!("{}Concrete", struct_item.ident),
                                Span::call_site()
                            );
                            
                            let new_struct = ItemStruct {
                                attrs: struct_item.attrs.clone(),
                                vis: struct_item.vis.clone(),
                                struct_token: struct_item.struct_token,
                                ident: new_struct_name.clone(),
                                generics: Generics {
                                    lt_token: None,
                                    params: Punctuated::new(),
                                    gt_token: None,
                                    where_clause: None,
                                },
                                fields: Fields::Named(FieldsNamed {
                                    brace_token: syn::token::Brace::default(),
                                    named: Punctuated::from_iter(new_fields),
                                }),
                                semi_token: None,
                            };
                            
                            items_to_insert.push(Item::Struct(new_struct));
                        }
                    }
                }
            }
        }
        
        for new_item in items_to_insert {
            file.items.insert(0, new_item);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets generic unions containing struct fields that use associated type projections. It extracts the associated type projection from struct fields, attempts to find concrete type bounds from trait implementations for the union's generic parameters, and replaces the projection with the concrete type. If no concrete bound is found, it uses a dummy type (u32). It then creates a new concrete struct with the resolved types and updates the union field to use this new struct. This transformation aims to trigger normalization issues in the compiler when handling unions with associated types, potentially exposing bugs in type resolution and trait system interactions."
    }
}

struct UnionAssociatedTypeVisitor {
    trait_impls: Vec<(String, String, Type)>,
}

impl VisitMut for UnionAssociatedTypeVisitor {
    fn visit_item_impl_mut(&mut self, node: &mut ItemImpl) {
        if let Some((_, trait_path, _)) = &node.trait_ {
            let trait_name = trait_path.segments.last().unwrap().ident.to_string();
            
            for impl_item in &node.items {
                if let ImplItem::Type(type_item) = impl_item {
                    let assoc_name = type_item.ident.to_string();
                    if let Type::Path(type_path) = &type_item.ty {
                        self.trait_impls.push((
                            trait_name.clone(),
                            assoc_name,
                            Type::Path(type_path.clone())
                        ));
                    }
                }
            }
        }
        visit_mut::visit_item_impl_mut(self, node);
    }
}

fn find_struct_by_type<'a>(file: &'a File, path: &SynPath) -> Option<&'a ItemStruct> {
    for item in &file.items {
        if let Item::Struct(struct_item) = item {
            if struct_item.ident == path.segments.last().unwrap().ident {
                return Some(struct_item);
            }
        }
    }
    None
}

fn extract_associated_projection(type_path: &mut TypePath) -> Option<(String, String, String)> {
    for segment in &mut type_path.path.segments {
        if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
            for arg in &mut args.args {
                if let GenericArgument::AssocType(assoc) = arg {
                    if let Type::Path(assoc_path) = &assoc.ty {
                        if assoc_path.path.segments.len() >= 3 {
                            let segments: Vec<_> = assoc_path.path.segments.iter().collect();
                            if let PathArguments::AngleBracketed(inner_args) = &segments[0].arguments {
                                if let Some(GenericArgument::Type(Type::Path(trait_type))) = inner_args.args.first() {
                                    let trait_name = segments[0].ident.to_string();
                                    let assoc_name = segments[2].ident.to_string();
                                    let generic_param = trait_type.path.segments.last().unwrap().ident.to_string();
                                    return Some((trait_name, assoc_name, generic_param));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    None
}

fn find_concrete_type_for_assoc(
    trait_1impls: &[(String, String, Type)],
    trait_name: &str,
    assoc_name: &str,
    generic_param: &str,
    union_item: &ItemUnion
) -> Option<TypePath> {
    for param in &union_item.generics.params {
        if let GenericParam::Type(type_param) = param {
            if type_param.ident.to_string() == *generic_param {
                for (impl_trait, impl_assoc, concrete_type) in trait_1impls {
                    if impl_trait == trait_name && impl_assoc == assoc_name {
                        if let Type::Path(type_path) = concrete_type {
                            return Some(type_path.clone());
                        }
                    }
                }
            }
        }
    }
    None
}