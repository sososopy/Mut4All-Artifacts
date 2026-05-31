//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_NonType_Template_Default_Arg_With_Var_240
 */ 
class MutatorFrontendAction_240 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(240)

private:
    class MutatorASTConsumer_240 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_240(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_NonType_Template_Default_Arg_With_Var_240.h"

// ========================================================================================================
#define MUT240_OUTPUT 1

void MutatorFrontendAction_240::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *param = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("param")) {
        if (!param || !Result.Context->getSourceManager().isWrittenInMainFile(param->getLocation()))
            return;

        if (!param->hasDefaultArgument())
            return;

        const clang::TemplateArgument &defaultArg = param->getDefaultArgument();
        if (defaultArg.getKind() != clang::TemplateArgument::Integral)
            return;

        llvm::APSInt value = defaultArg.getAsIntegral();
        std::string valueStr = value.toString(10, true);
        std::string varName = "x";

        SourceRange range = param->getDefaultArgument().getSourceRange();
        Rewrite.ReplaceText(range, varName);

        const DeclContext *dc = param->getParent();
        SourceLocation insertLoc;

        if (const NamespaceDecl *ns = dyn_cast<NamespaceDecl>(dc)) {
            insertLoc = ns->getBeginLoc();
        } else if (isa<TranslationUnitDecl>(dc)) {
            insertLoc = param->getLocation();
        } else {
            const CXXRecordDecl *record = dyn_cast<CXXRecordDecl>(dc);
            if (record) {
                insertLoc = record->getLocation();
            }
        }

        if (insertLoc.isValid()) {
            std::string decl = "int " + varName + " = " + valueStr + ";";
            Rewrite.InsertText(insertLoc, decl + "\n", true, true);
        }
    }
}

void MutatorFrontendAction_240::MutatorASTConsumer_240::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = templateTypeParmDecl(isNonType(), hasDefaultArgument()).bind("param");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}