//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Decltype_Return_Type_With_Transformed_Expression_24
 */ 
class MutatorFrontendAction_24 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(24)

private:
    class MutatorASTConsumer_24 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_24(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Decltype_Return_Type_With_Transformed_Expression_24.h"

// ========================================================================================================
#define MUT24_OUTPUT 1

void MutatorFrontendAction_24::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DT = Result.Nodes.getNodeAs<clang::DecltypeType>("decltypeType")) {
        if (!DT || !Result.Context->getSourceManager().isWrittenInMainFile(DT->getLocation()))
            return;

        auto decltext = stringutils::rangetoStr(*Result.SourceManager, DT->getSourceRange());
        if (decltext.size() < 10 || decltext.substr(0, 9) != "decltype(" || decltext.back() != ')')
            return;

        std::string param = decltext.substr(9, decltext.size() - 10);
        std::vector<std::string> transforms = {"&", "*", "++", "--"};
        std::string op = getrandom::getRandomElement(transforms);
        std::string new_type = "decltype(" + op + param + ")";
        Rewrite.ReplaceText(DT->getSourceRange(), new_type);
    }
}
  
void MutatorFrontendAction_24::MutatorASTConsumer_24::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(
        hasDescendant(
            lambdaExpr(
                hasTrailingReturnType(
                    decltypeType().bind("decltypeType")
                )
            ).bind("lambda")
        )
    ).bind("lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}