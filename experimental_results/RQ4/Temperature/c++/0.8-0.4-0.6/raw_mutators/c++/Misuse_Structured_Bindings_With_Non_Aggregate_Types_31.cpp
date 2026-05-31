//header file
#pragma once
#include "Mutator_base.h"

/**
 * misuse_structured_bindings_with_non_aggregate_types_31
 */ 
class MutatorFrontendAction_31 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(31)

private:
    class MutatorASTConsumer_31 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_31(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/misuse_structured_bindings_with_non_aggregate_types_31.h"

// ========================================================================================================
#define MUT31_OUTPUT 1

void MutatorFrontendAction_31::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("structuredBinding")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      // Check if the variable is a structured binding
      if (VD->isStructuredBinding() && !VD->getType()->isAggregateType()) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   VD->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string mutation = "const auto& [...pack] = " + VD->getNameAsString() + "; (pack, ...);";
        declaration.insert(declaration.find(";"), "\n/*mut31*/" + mutation);
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_31::MutatorASTConsumer_31::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(isStructuredBinding()).bind("structuredBinding");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}