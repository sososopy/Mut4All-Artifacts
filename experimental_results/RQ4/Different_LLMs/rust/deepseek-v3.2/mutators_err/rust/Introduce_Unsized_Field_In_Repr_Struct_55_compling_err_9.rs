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

pub struct Introduce_Unsized_Field_In_Repr_Struct_55;

impl Mutator for Introduce_Unsized_Field_In_Repr_Struct_55 {
    fn name(&self) -> &str {
        "Introduce_Unsized_Field_In_Repr_Struct_55"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut repr_structs = Vec::new();
        let mut size_of_calls = Vec::new();
        let mut rng = thread_rng();

        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                let struct_item_clone = struct_item.clone();
                for attr in &struct_item_clone.attrs {
                    if attr.path().is_ident("repr") {
                        let tokens = attr.meta.to_token_stream();
                        let parser = syn::parse2::<syn::Meta>(tokens).unwrap();
                        if let syn::Meta::List(list) = parser {
                            if list.path.is_ident("repr") {
                                for token in list.tokens.clone().into_iter() {
                                    let path = syn::parse2::<syn::Path>(token.into()).unwrap();
                                    if path.is_ident("C") || path.is_ident("packed") {
                                        repr_structs.push(struct_item_clone.clone());
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        struct SizeOfVisitor {
            calls: Vec<*mut ExprCall>,
        }
        impl<'ast> Visit<'ast> for SizeOfVisitor {
            fn visit_expr_call(&mut self, i: &'ast ExprCall) {
                if let Expr::Path(path) = &*i.func {
                    if path.path.segments.last().map(|seg| seg.ident == "size_of").unwrap_or(false) {
                        self.calls.push(i as *const _ as *mut _);
                    }
                }
                syn::visit::visit_expr_call(self, i);
            }
        }
        let mut visitor = SizeOfVisitor { calls: Vec::new() };
        visitor.visit_file(&*file);
        size_of_calls = visitor.calls;

        if repr_structs.is_empty() {
            let new_struct = parse_quote! {
                #[repr(C)]
                struct MutatedStruct {
                    sized_field: u8,
                    unsized_field: str,
                }
            };
            file.items.push(Item::Struct(new_struct));
            let size_of_call: Stmt = parse_quote! {
                let _size = std::mem::size_of::<MutatedStruct>();
            };
            for item in &mut file.items {
                if let Item::Fn(func) = item {
                    if func.sig.ident == "main" {
                        func.block.stmts.push(size_of_call);
                        break;
                    }
                }
            }
        } else {
            let target_struct = repr_structs.choose(&mut rng).unwrap();
            let mut sized_fields = Vec::new();
            for field in &target_struct.fields {
                if !is_unsized_type(&field.ty) {
                    sized_fields.push(field);
                }
            }
            if !sized_fields.is_empty() {
                let target_field = sized_fields.choose(&mut rng).unwrap();
                let index = target_struct.fields.iter().position(|f| f == *target_field).unwrap();
                let unsized_type: Type = parse_quote! { str };
                for item in &mut file.items {
                    if let Item::Struct(struct_item) = item {
                        if struct_item.ident == target_struct.ident {
                            struct_item.fields.iter_mut().nth(index).unwrap().ty = unsized_type;
                            break;
                        }
                    }
                }
            }
            if size_of_calls.is_empty() {
                let struct_name = &target_struct.ident;
                let size_of_call: Stmt = parse_quote! {
                    let _size = std::mem::size_of::<#struct_name>();
                };
                for item in &mut file.items {
                    if let Item::Fn(func) = item {
                        if func.sig.ident == "main" {
                            func.block.stmts.push(size_of_call);
                            break;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets structs with repr(C) or repr(packed). It either modifies an existing repr struct by replacing a sized field with an unsized type (like str), or introduces a new repr(C) struct with both sized and unsized fields. It ensures the struct is used in a size_of call, either by adding such a call to main or leveraging existing ones. This transformation stresses the compiler's layout computation for unsized types in repr structs, potentially triggering layout errors or ICEs."
    }
}

fn is_unsized_type(ty: &Type) -> bool {
    match ty {
        Type::Slice(_) => true,
        Type::Path(TypePath { path, .. }) => {
            let seg = path.segments.last();
            seg.map(|s| s.ident == "str").unwrap_or(false)
        }
        _ => false,
    }
}