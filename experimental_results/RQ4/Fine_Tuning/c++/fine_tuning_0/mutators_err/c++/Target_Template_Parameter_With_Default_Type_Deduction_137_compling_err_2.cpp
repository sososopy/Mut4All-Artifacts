//header file
#pragma once
#include "Mutator_base.h"

/**
 * Target_Template_Parameter_With_Default_Type_Deduction_137
 */ 
class MutatorFrontendAction_137 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(137)

private:
    class MutatorASTConsumer_137 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_137(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Target_Template_Parameter_With_Default_Type_Deduction_137.h"

// ========================================================================================================
#define MUT137_OUTPUT 1

void MutatorFrontendAction_137::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("TemplateTypeParmWithDefault")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto default_type = MT->getDefaultArgument();
      auto default_type_str = stringutils::rangetoStr(
          *(Result.SourceManager), default_type->getSourceRange());
      llvm::outs() << default_type_str << '\n';
      //Perform mutation on the source code text by applying string replacement
      if (default_type_str.find("decltype") == string::npos)
        return;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(default_type->getBeginLoc(), 8, "int");
    }
}
  
void MutatorFrontendAction_137::MutatorASTConsumer_137::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateTypeParmDecl(hasDefaultArgument()).bind("TemplateTypeParmWithDefault");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}