use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemImpl, ImplItem, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Insert_Derive_Macro_In_Trait_Impl_Local_Item_255;

impl Mutator for Insert_Derive_Macro_In_Trait_Impl_Local_Item_255 {
    fn name(&self) -> &str {
        "Insert_Derive_Macro_In_Trait_Impl_Local_Item_255"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(method) = impl_item {
                        let mut visitor = LocalItemVisitor {
                            method_block: method.block.clone(),
                            trait_impl_context: true,
                        };
                        visitor.visit_block_mut(&mut method.block);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts a derive macro attribute (e.g., serde::Deserialize) on a local struct or enum defined inside a trait implementation method. It specifically targets procedural macro-related traits. If a local item exists, it adds the derive attribute; otherwise, it creates a new local struct with a single field using an in-scope type and applies the derive attribute. This aims to replicate scenarios where derive macros expand inside trait impl methods, potentially causing recursive expansion or stack overflow."
    }
}

struct LocalItemVisitor {
    method_block: syn::Block,
    trait_impl_context: bool,
}

impl VisitMut for LocalItemVisitor {
    fn visit_stmt_mut(&mut self, stmt: &mut Stmt) {
        if let Stmt::Item(item) = stmt {
            if let Item::Struct(struct_item) = item {
                if !struct_item.attrs.iter().any(|attr| attr.path().is_ident("derive")) {
                    let derive_attr: syn::Attribute = parse_quote! {
                        #[derive(serde::Deserialize)]
                    };
                    struct_item.attrs.insert(0, derive_attr);
                }
            } else if let Item::Enum(enum_item) = item {
                if !enum_item.attrs.iter().any(|attr| attr.path().is_ident("derive")) {
                    let derive_attr: syn::Attribute = parse_quote! {
                        #[derive(serde::Deserialize)]
                    };
                    enum_item.attrs.insert(0, derive_attr);
                }
            }
        }
        syn::visit_mut::visit_stmt_mut(self, stmt);
    }

    fn visit_block_mut(&mut self, block: &mut syn::Block) {
        let mut has_local_item = false;
        for stmt in &block.stmts {
            if let Stmt::Item(item) = stmt {
                if let Item::Struct(_) = item {
                    has_local_item = true;
                    break;
                }
                if let Item::Enum(_) = item {
                    has_local_item = true;
                    break;
                }
            }
        }
        if !has_local_item && self.trait_impl_context {
            let new_struct: ItemStruct = parse_quote! {
                #[derive(serde::Deserialize)]
                struct LocalItem { field: i32 }
            };
            let new_stmt = Stmt::Item(Item::Struct(new_struct));
            block.stmts.insert(0, new_stmt);
        }
        syn::visit_mut::visit_block_mut(self, block);
    }
}