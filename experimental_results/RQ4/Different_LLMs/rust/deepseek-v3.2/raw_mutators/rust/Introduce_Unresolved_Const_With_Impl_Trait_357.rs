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

pub struct Introduce_Unresolved_Const_With_Impl_Trait_357;

impl Mutator for Introduce_Unresolved_Const_With_Impl_Trait_357 {
    fn name(&self) -> &str {
        "Introduce_Unresolved_Const_With_Impl_Trait_357"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_generic_const_exprs = false;
        for attr in &file.attrs {
            if let Meta::NameValue(name_value) = &attr.meta {
                if name_value.path.is_ident("feature") {
                    if let Expr::Lit(expr_lit) = &name_value.value {
                        if let Lit::Str(lit_str) = &expr_lit.lit {
                            if lit_str.value() == "generic_const_exprs" {
                                has_generic_const_exprs = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
        
        if !has_generic_const_exprs {
            file.attrs.insert(0, parse_quote!(#![feature(generic_const_exprs)]));
        }

        let mut structs_with_const_fields = Vec::new();
        let mut impls_for_structs = Vec::new();
        
        for item in &file.items {
            if let Item::Struct(item_struct) = item {
                let mut has_const_field = false;
                for field in &item_struct.fields {
                    if let Type::Array(type_array) = &field.ty {
                        has_const_field = true;
                        break;
                    }
                }
                if has_const_field {
                    structs_with_const_fields.push((item_struct.ident.clone(), item_struct.clone()));
                }
            }
            if let Item::Impl(item_impl) = item {
                if let Some((_, path)) = &item_impl.trait_ {
                    continue;
                }
                if let Type::Path(type_path) = &*item_impl.self_ty {
                    impls_for_structs.push((type_path.path.get_ident().cloned(), item_impl.clone()));
                }
            }
        }

        for (struct_ident, struct_item) in structs_with_const_fields {
            let mut modified_struct = false;
            let mut unresolved_const_name = Ident::new("UNDECLARED_CONST", Span::call_site());
            
            for item in &mut file.items {
                if let Item::Struct(item_struct) = item {
                    if item_struct.ident == struct_ident {
                        for field in &mut item_struct.fields {
                            if let Type::Array(type_array) = &mut field.ty {
                                if let Expr::Lit(expr_lit) = &*type_array.len {
                                    if let Lit::Int(lit_int) = &expr_lit.lit {
                                        let unresolved_expr: Expr = parse_quote!(#unresolved_const_name);
                                        type_array.len = Box::new(unresolved_expr);
                                        modified_struct = true;
                                    }
                                }
                            }
                        }
                        break;
                    }
                }
            }

            if modified_struct {
                let mut has_impl_with_method = false;
                for (impl_ident, impl_item) in &impls_for_structs {
                    if impl_ident.as_ref() == Some(&struct_ident) {
                        for impl_item in &impl_item.items {
                            if let ImplItem::Fn(method) = impl_item {
                                if let ReturnType::Type(_, return_type) = &method.sig.output {
                                    if let Type::ImplTrait(_) = &**return_type {
                                        has_impl_with_method = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }

                if !has_impl_with_method {
                    let mut found_impl = false;
                    for item in &mut file.items {
                        if let Item::Impl(item_impl) = item {
                            if let Type::Path(type_path) = &*item_impl.self_ty {
                                if type_path.path.get_ident() == Some(&struct_ident) {
                                    let new_method: ImplItem = parse_quote! {
                                        fn get_iter(&self) -> impl Iterator<Item = &u8> {
                                            self.0.iter()
                                        }
                                    };
                                    item_impl.items.push(new_method);
                                    found_impl = true;
                                    break;
                                }
                            }
                        }
                    }

                    if !found_impl {
                        let new_impl: Item = parse_quote! {
                            impl #struct_ident {
                                fn get_iter(&self) -> impl Iterator<Item = &u8> {
                                    self.0.iter()
                                }
                            }
                        };
                        file.items.push(new_impl);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces unresolved constants in const generic expressions within struct definitions and ensures there's a method returning an opaque impl Trait that borrows the mutated field. It first adds the generic_const_exprs feature if missing, then identifies structs with array fields. For each such struct, it replaces array size literals with an unresolved identifier. If the struct doesn't have an impl with a method returning impl Trait, it adds one that borrows the field. This creates the exact conditions that trigger the bug: unresolved constant in const generic context combined with opaque return type and borrow checking."
    }
}