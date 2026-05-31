//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Nested_Container_Init_List_418
 */ 
class MutatorFrontendAction_418 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(418)

private:
    class MutatorASTConsumer_418 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_418(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Replace_Nested_Container_Init_List_418.h"

// ========================================================================================================
#define MUT418_OUTPUT 1

void MutatorFrontendAction_418::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *varDecl = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
        // Filter nodes in header files
        if (!varDecl || !Result.Context->getSourceManager().isWrittenInMainFile(varDecl->getLocation()))
            return;

        QualType type = varDecl->getType();
        if (!isNestedContainer(type, *Result.Context))
            return;

        if (auto *init = varDecl->getInit()) {
            if (auto *ile = dyn_cast<clang::InitListExpr>(init)) {
                if (ile->getNumArgs() > 0) {
                    // Replace the initializer list with empty braces
                    Rewrite.ReplaceText(ile->getSourceRange(), "{}");
                }
            }
        }
    }
}

// Helper functions
namespace {
    bool isContainerType(clang::QualType type, clang::ASTContext &Context) {
        if (auto *TST = type->getAs<clang::TemplateSpecializationType>()) {
            clang::TemplateName TemplateName = TST->getTemplateName();
            if (auto *TD = TemplateName.getAsTemplateDecl()) {
                if (TD && TD->getDeclContext()->getRedeclContext()->getTranslationUnitDecl() == Context.getTranslationUnitDecl()) {
                    std::string name = TD->getNameAsString();
                    clang::NamespaceDecl *NS = clang::dyn_cast<clang::NamespaceDecl>(TD->getDeclContext());
                    if (NS && NS->getNameAsString() == "std") {
                        return (name == "vector" || name == "array" || name == "map" || 
                                name == "unordered_map" || name == "list" || name == "set" ||
                                name == "unordered_set");
                    }
                }
            }
        }
        return false;
    }

    clang::QualType getElementType(clang::QualType type, clang::ASTContext &Context) {
        if (auto *TST = type->getAs<clang::TemplateSpecializationType>()) {
            clang::TemplateName TemplateName = TST->getTemplateName();
            if (auto *TD = TemplateName.getAsTemplateDecl()) {
                std::string name = TD->getNameAsString();
                if (name == "map" || name == "unordered_map") {
                    if (TST->template_arguments().size() >= 2) {
                        return TST->template_arguments()[1].get();
                    }
                } else {
                    if (TST->template_arguments().size() >= 1) {
                        return TST->template_arguments()[0].get();
                    }
                }
            }
        }
        return clang::QualType();
    }

    bool isNestedContainer(clang::QualType type, clang::ASTContext &Context) {
        if (!isContainerType(type, Context))
            return false;

        clang::QualType elementType = getElementType(type, Context);
        if (elementType.isNull())
            return false;

        return isContainerType(elementType, Context);
    }
}

void MutatorFrontendAction_418::MutatorASTConsumer_418::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define the ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = varDecl(hasInitializer(initListExpr())).bind("varDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}