//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Default_Arguments_In_Nested_Namespaces_230
 */ 
class MutatorFrontendAction_230 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(230)

private:
    class MutatorASTConsumer_230 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_230(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        std::string newNamespaceName;
        std::string newTypeName;
    };
};

//source file
#include "../include/modify_template_default_arguments_in_nested_namespaces_230.h"

// ========================================================================================================
#define MUT230_OUTPUT 1

void MutatorFrontendAction_230::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClass")) {
        if (!NTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NTD->getLocation()))
            return;

        const NamespaceDecl *parentNamespace = dyn_cast<NamespaceDecl>(NTD->getDeclContext());
        if (!parentNamespace)
            return;

        std::string parentNamespaceName = parentNamespace->getNameAsString();
        newNamespaceName = parentNamespaceName + "Dup";
        newTypeName = "NewType";

        std::string newNamespaceDeclaration = "namespace " + newNamespaceName + " { using " + newTypeName + " = double; }";
        std::string newDefaultArgument = newNamespaceName + "::" + newTypeName;

        const TemplateParameterList *params = NTD->getTemplateParameters();
        if (params->size() > 0) {
            const NamedDecl *param = params->getParam(0);
            if (const TemplateTypeParmDecl *typeParam = dyn_cast<TemplateTypeParmDecl>(param)) {
                if (typeParam->hasDefaultArgument()) {
                    SourceLocation defaultArgLoc = typeParam->getDefaultArgumentLoc();
                    Rewrite.ReplaceText(defaultArgLoc, newDefaultArgument);
                }
            }
        }

        SourceLocation insertLocation = parentNamespace->getRBraceLoc();
        Rewrite.InsertTextBefore(insertLocation, newNamespaceDeclaration + "\n");
    }
}

void MutatorFrontendAction_230::MutatorASTConsumer_230::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl(
        hasAncestor(namespaceDecl())).bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}