//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unnamed_Namespace_With_Function_114
 */ 
class MutatorFrontendAction_114 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(114)

private:
    class MutatorASTConsumer_114 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_114(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_unnamed_namespace_with_function_114.h"

// ========================================================================================================
#define MUT114_OUTPUT 1

void MutatorFrontendAction_114::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody()) {
        auto functionBody = stringutils::rangetoStr(*(Result.SourceManager),
                                                    FD->getSourceRange());
        std::string unnamedNamespace = "\nnamespace {\nvoid hiddenFunction() {\n";
        unnamedNamespace += functionBody;
        unnamedNamespace += "\n}\n}\n";

        Rewrite.InsertTextBefore(FD->getBeginLoc(), "/*mut114*/" + unnamedNamespace);
      }
    }
}
  
void MutatorFrontendAction_114::MutatorASTConsumer_114::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}