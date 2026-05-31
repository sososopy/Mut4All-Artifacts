//header file
#pragma once
#include "Mutator_base.h"

/**
 * NestedLambdaPackExpansion_452
 */ 
class MutatorFrontendAction_452 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(452)

private:
    class MutatorASTConsumer_452 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_452(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> capturedTemplates;
    };
};

//source file
#include "../include/NestedLambdaPackExpansion_452.h"

// ========================================================================================================
#define MUT452_OUTPUT 1

void MutatorFrontendAction_452::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("VariadicFunction")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (!FD->isVariadic() && !FD->getTemplateSpecializationKind())
            return;

        if (!FD->getBody())
            return;

        SourceLocation startLoc = FD->getBody()->getBeginLoc();
        SourceLocation endLoc = FD->getBody()->getEndLoc();

        std::string bodyText = stringutils::rangetoStr(*Result.SourceManager, FD->getBody()->getSourceRange());

        std::string outerLambda = "auto outer = [&](auto... A1) {\n";
        std::string innerLambda = "  auto inner = [&](auto... A2) {\n";
        std::string foldExp = "    (std::void_t<A1, A2>{}, ...);\n";
        std::string innerCall = "  };\n  inner(Ts...);\n";
        std::string outerCall = "};\nouter(Ts...);";

        std::string mutatedBody = outerLambda + innerLambda + foldExp + innerCall + outerCall;

        // Replace the original body with the new nested lambda structure
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getBody()->getSourceRange()), mutatedBody);
    }
}
  
void MutatorFrontendAction_452::MutatorASTConsumer_452::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isVariadic(), isTemplateInstantiation()).bind("VariadicFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}