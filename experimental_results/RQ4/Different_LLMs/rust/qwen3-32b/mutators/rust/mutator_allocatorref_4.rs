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

pub struct Mutator_AllocatorRef_4;

fn path_to_string(path: &SynPath) -> String {
    path.segments.iter()
        .map(|seg| seg.ident.to_string())
        .collect::<Vec<_>>()
        .join("::")
}

impl Mutator for Mutator_AllocatorRef_4 {
    fn name(&self) -> &str {
        "Mutator_AllocatorRef_4"
    }
    fn mutate(&self, file: &mut syn::File) {
        if !file.attrs.iter().any(|attr| {
            if let syn::Meta::List(list) = &attr.meta {
                if list.path.is_ident("feature") {
                    let s = list.tokens.to_string();
                    s.contains("allocator_api")
                } else {
                    false
                }
            } else {
                false
            }
        }) {
            let feature_attr = syn::Attribute {
                pound_token: Default::default(),
                style: syn::AttrStyle::Outer,
                bracket_token: Default::default(),
                meta: syn::Meta::List(syn::MetaList {
                    path: syn::Path::from(syn::Ident::new("feature", Span::call_site())),
                    delimiter: syn::MacroDelimiter::Bracket(Default::default()),
                    tokens: quote! { allocator_api }.into_token_stream(),
                }),
            };
            file.attrs.insert(0, feature_attr);
        }

        let use_stmt = parse_quote! {
            use std::alloc::{Allocator, Layout, AllocError, Global, NonNull};
        };
        file.items.insert(0, syn::Item::Use(use_stmt));

        let zst_allocator = parse_quote! {
            struct ZSTAllocator;
        };
        file.items.insert(1, syn::Item::Struct(zst_allocator));

        let impl_allocator = parse_quote! {
            impl Allocator for &ZSTAllocator {
                fn allocate(&self, layout: Layout) -> Result<NonNull<[u8]>, AllocError> {
                    Global.allocate(layout)
                }

                unsafe fn deallocate(&self, ptr: NonNull<u8>, layout: Layout) {
                    Global.deallocate(ptr, layout)
                }

                fn allocate_zeroed(&self, layout: Layout) -> Result<NonNull<[u8]>, AllocError> {
                    Global.allocate_zeroed(layout)
                }

                fn realloc(
                    &self,
                    ptr: NonNull<u8>,
                    layout: Layout,
                    new_size: usize,
                ) -> Result<NonNull<[u8]>, AllocError> {
                    Global.realloc(ptr, layout, new_size)
                }

                fn shrink(
                    &self,
                    ptr: NonNull<u8>,
                    old_layout: Layout,
                    new_size: usize,
                ) -> Result<NonNull<[u8]>, AllocError> {
                    Global.shrink(ptr, old_layout, new_size)
                }

                fn grow(
                    &self,
                    ptr: NonNull<u8>,
                    old_layout: Layout,
                    new_size: usize,
                ) -> Result<NonNull<[u8]>, AllocError> {
                    Global.grow(ptr, old_layout, new_size)
                }
            }
        };
        file.items.insert(2, syn::Item::Impl(impl_allocator));

        struct BoxReplacer;
        impl VisitMut for BoxReplacer {
            fn visit_expr_mut(&mut self, expr: &mut syn::Expr) {
                if let syn::Expr::Call(call) = expr {
                    if let syn::Expr::Path(path) = &*call.func {
                        if path_to_string(&path.path) == "Box::new" {
                            if let Some(arg) = call.args.first() {
                                let new_expr = parse_quote! {
                                    Box::<_, &ZSTAllocator>::new_in(#arg, &ZSTAllocator)
                                };
                                *expr = new_expr;
                            }
                        }
                    }
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }

        let mut visitor = BoxReplacer;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a reference-based allocator for non-ZST types, creating a mismatch in pointer/pointee metadata. By replacing Box<T> with Box<T, &ZSTAllocator>, it stresses the compiler's handling of allocator types and debuginfo metadata, potentially exposing bugs in allocator trait resolution or metadata tracking."
    }
}