//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Auto_With_Decltype_Auto_In_Template_Function_Trailing_Return_Type_429
 */ 
class MutatorFrontendAction_429 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(429)

private:
    class MutatorASTConsumer_429 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_429(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Replace_Auto_With_Decltype_Auto_In_Template_Function_Trailing_Return_Type_429.h"

// ========================================================================================================
#define MUT429_OUTPUT 1

void MutatorFrontendAction_429::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("TemplateFunc")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto returntype = MT->getReturnType();
      if (returntype.getAsString() != "auto")
        return;
      auto definition = stringutils::rangetoStr(*(Result.SourceManager),
                                                MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      llvm::outs() << "mut429: " << definition << '\n';
      llvm::outs() << "mut429: " << returntype.getAsString() << '\n';
      definition.replace(definition.find("auto"), 4, "decltype(auto)");
      llvm::outs() << "mut429: " << definition << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), definition);
    }
}
  
void MutatorFrontendAction_429::MutatorASTConsumer_429::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasTrailingReturnType(), isTemplateInstantiation()).bind("TemplateFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}