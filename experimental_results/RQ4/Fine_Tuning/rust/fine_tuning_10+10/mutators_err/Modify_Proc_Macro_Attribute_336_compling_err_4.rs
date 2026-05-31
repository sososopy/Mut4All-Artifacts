use crate::mutator::Mutator;

pub struct Modify_Proc_Macro_Attribute_336;

impl Mutator for Modify_Proc_Macro_Attribute_336 {
    fn name(&self) -> &str {
        "Modify_Proc_Macro_Attribute_336"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_identity_function = false;
        let existing_fn = find_existing_function(file);

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.attrs.iter().any(|attr| attr.path().is_ident("proc_macro_attribute")) {
                    for stmt in &mut item_fn.block.stmts {
                        if let Stmt::Expr(Expr::Macro(expr_macro), _) = stmt {
                            if expr_macro.mac.path.is_ident("panic") {
                                if let Some(ref existing_fn) = existing_fn {
                                    expr_macro.mac.path = existing_fn.clone();
                                } else {
                                    expr_macro.mac.path = parse_quote!(identity_function);
                                    has_identity_function = true;
                                }
                            }
                        }
                    }
                }
            }
        }

        if has_identity_function {
            file.items.push(parse_quote! {
                fn identity_function(input: TokenStream) -> TokenStream {
                    input
                }
            });
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions annotated with #[proc_macro_attribute]. It replaces 'panic!()' calls with a call to an existing function that returns a TokenStream, or introduces an identity function if no suitable function exists. This change modifies the macro's behavior from immediate panic to a transformation of the input."
    }
}

fn find_existing_function(file: &syn::File) -> Option<SynPath> {
    for item in &file.items {
        if let Item::Fn(item_fn) = item {
            if let ReturnType::Type(_, ref ty) = item_fn.sig.output {
                if let Type::Path(TypePath { path, .. }) = &**ty {
                    if path.is_ident("TokenStream") {
                        return Some(item_fn.sig.ident.clone().into());
                    }
                }
            }
        }
    }
    None
}