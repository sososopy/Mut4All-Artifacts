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
        // Add feature gate if not present
        if !file.attrs.iter().any(|attr| {
            if let syn::Meta::List(list) = &attr.meta {
                if list.path.is_ident("feature") {
                    if let Some(tokens) = &list.tokens {
                        let s = tokens.to_string();
                        s.contains("allocator_api")
                    } else {
                        false
                    }
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
                    nested: {
                        let mut nested = syn::punctuated::Punctuated::new();
                        nested.push(syn::NestedMeta::Meta(syn::Meta::NameValue(syn::MetaNameValue {
                            path: syn::Path::from(syn::Ident::new("allocator_api", Span::call_site())),
                            eq_token: Default::default(),
                            value: syn::Expr::Lit(syn::ExprLit {
                                attrs: vec![],
                                lit: syn::Lit::Str(syn::LitStr::new("allocator_api", Span::call_site())),
                            }),
                        })));
                        nested
                    },
                }),
            };
            file.attrs.insert(0, feature_attr);
        }

        // Add use statement for Allocator, etc.
        let use_stmt = parse_quote! {
            use std::alloc::{Allocator, Layout, AllocError, Global, NonNull};
        };
        file.items.insert(0, syn::Item::Use(use_stmt));

        // Add ZSTAllocator struct
        let zst_allocator = parse_quote! {
            struct ZSTAllocator;
        };
        file.items.insert(1, syn::Item::Struct(zst_allocator));

        // Add Allocator impl for &ZSTAllocator
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

        // Replace Box::new with Box::<_, &ZSTAllocator>::new_in
        struct BoxReplacer;
        impl VisitMut for BoxReplacer {
            fn visit_expr(&mut self, expr: &mut syn::Expr) {
                if let syn::Expr::Call(call) = expr {
                    if let syn::Expr::Path(path) = &*call.func {
                        if path_to_string(path) == "Box::new" {
                            if let Some(arg) = call.args.first() {
                                let new_expr = parse_quote! {
                                    Box::<_, &ZSTAllocator>::new_in(#arg, &ZSTAllocator)
                                };
                                *expr = new_expr;
                            }
                        }
                    }
                }
                self.visit_expr_mut(expr);
            }
        }

        let mut visitor = BoxReplacer;
        visitor.visit_file(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a reference-based allocator for non-ZST types, creating a mismatch in pointer/pointee metadata. By replacing Box<T> with Box<T, &ZSTAllocator>, it stresses the compiler's handling of allocator types and debuginfo metadata, potentially exposing bugs in allocator trait resolution or metadata tracking."
    }
}