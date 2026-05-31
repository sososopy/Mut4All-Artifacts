//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Class_With_Enum_2
 */ 
class MutatorFrontendAction_{{bugno}} : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER({{bugno}})

private:
    class MutatorASTConsumer_{{bugno}} : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_{{bugno}}(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> enumNames;
    };
};

//source file
#include "../include/Replace_Template_Class_With_Enum_2.h"

// ========================================================================================================
#define MUT{{bugno}}_OUTPUT 1

void MutatorFrontendAction_{{bugno}}::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *enumDecl = Result.Nodes.getNodeAs<clang::EnumDecl>("enumDecl")) {
        if (!enumDecl || !Result.Context->getSourceManager().isWrittenInMainFile(enumDecl->getLocation()))
            return;
        if (enumDecl->isScoped()) {
            std::string enumName = enumDecl->getNameAsString();
            if (!enumName.empty()) {
                enumNames.push_back(enumName);
            }
        }
    } else if (auto *funcTemplate = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("funcTemplate")) {
        if (!funcTemplate || !Result.Context->getSourceManager().isWrittenInMainFile(funcTemplate->getLocation()))
            return;
        for (auto param : funcTemplate->getTemplateParameters()) {
            if (auto typeParam = dyn_cast<clang::TypeTemplateParmDecl>(param)) {
                SourceRange paramRange = typeParam->getSourceRange();
                if (!enumNames.empty()) {
                    std::string newParam = "typename " + enumNames[0];
                    Rewrite.ReplaceText(paramRange, newParam);
                }
                break;
            }
        }
    }
}

void MutatorFrontendAction_{{bugno}}::MutatorASTConsumer_{{bugno}}::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher1 = enumDecl(isScoped()).bind("enumDecl");
    DeclarationMatcher matcher2 = functionTemplateDecl(hasAnyParameter(hasType(memberPointerType()))).bind("funcTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}