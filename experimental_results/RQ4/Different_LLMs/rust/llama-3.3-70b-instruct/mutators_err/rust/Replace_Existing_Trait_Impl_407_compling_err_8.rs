use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet,
    default,
    fs,
    ops::Range,
    panic,
    path::Path,
    process::Command,
};
use syn::{
    BoundLifetimes,
    Expr,
    ExprCall,
    ExprPath,
    File,
    FnArg,
    GenericArgument,
    GenericParam,
    Ident,
    Item,
    ItemFn,
    ItemStruct,
    Lifetime,
    LifetimeParam,
    Local,
    Pat,
    PatType,
    Path as SynPath,
    PathArguments,
    ReturnType,
    Stmt,
    TraitBound,
    TraitBoundModifier,
    Type,
    TypeImplTrait,
    TypeParamBound,
    TypePath,
    parse_quote,
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

pub struct Replace_Existing_Trait_Impl_407;

impl Mutator for Replace_Existing_Trait_Impl_407 {
    fn name(&self) -> &str {
        "Replace_Existing_Trait_Impl_407"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                if let Some(trait_path) = get_trait_path(&impl_item) {
                    let new_impl = create_new_impl(&impl_item, &trait_path);
                    *item = Item::Impl(new_impl);
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the existing implementation of a trait for a type with a new implementation that has a different set of methods or modifies the existing methods to have different return types or parameters."
    }
}

fn get_trait_path(impl_item: &syn::ItemImpl) -> Option<&SynPath> {
    impl_item.trait_.as_ref().map(|(not, path, _)| path)
}

fn create_new_impl(impl_item: &syn::ItemImpl, trait_path: &SynPath) -> syn::ItemImpl {
    let mut new_impl = impl_item.clone();
    if let Some(path_segment) = trait_path.segments.last() {
        let new_method_name = format!("new_{}", path_segment.ident);
        let new_method = create_new_method(&new_method_name);
        new_impl.items.push(syn::ImplItem::Fn(new_method));
    }
    new_impl
}

fn create_new_method(method_name: &str) -> syn::ImplItemFn {
    let method_name = Ident::new(method_name, Span::call_site());
    let method_sig = syn::Signature {
        constness: None,
        asyncness: None,
        unsafety: None,
        abi: None,
        ident: method_name,
        generics: syn::Generics::default(),
        inputs: syn::punctuated::Punctuated::new(),
        output: syn::ReturnType::Default,
        variadic: None,
        fn_token: token::Fn(Span::call_site()),
        paren_token: Paren {
            span: Span::call_site(),
        },
    };
    let method_block = syn::Block {
        brace_token: token::Brace {
            span: Span::call_site(),
        },
        stmts: Vec::new(),
    };
    syn::ImplItemFn {
        attrs: Vec::new(),
        vis: syn::Visibility::Inherited,
        sig: method_sig,
        block: method_block,
        defaultness: syn::Defaultness::Final,
    }
}