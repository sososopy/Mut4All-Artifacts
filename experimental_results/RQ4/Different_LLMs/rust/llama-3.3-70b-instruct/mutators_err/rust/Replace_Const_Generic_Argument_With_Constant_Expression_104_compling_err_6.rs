impl Mutator for Replace_Const_Generic_Argument_With_Constant_Expression_104 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Argument_With_Constant_Expression_104"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for arg in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = arg {
                        if let Type::Path(TypePath {
                            qself: None,
                            path: syn::Path {
                                leading_colon: None,
                                segments: segments,
                            },
                        }) = &*pat_type.ty
                        {
                            for segment in segments {
                                if let syn::PathSegment {
                                    ident: _,
                                    arguments: syn::PathArguments::AngleBracketed(args),
                                } = segment
                                {
                                    for arg in args.args {
                                        if let GenericArgument::Const(ref mut const_arg) = arg {
                                            let new_const_arg: Expr = parse_quote! { 5 };
                                            *const_arg = new_const_arg;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces constant generic arguments with constant expressions. It targets code constructs involving generic constants, such as functions or types with constant generic parameters, and replaces the constant generic argument with a simpler constant expression. This transformation aims to test the compiler's ability to handle different types of constant expressions as generic arguments and may help expose bugs related to constant generic expressions."
    }
}