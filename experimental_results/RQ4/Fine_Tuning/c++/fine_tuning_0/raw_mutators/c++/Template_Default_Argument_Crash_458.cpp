//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Default_Argument_Crash_458
 */ 
class MutatorFrontendAction_458 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(458)

private:
    class MutatorASTConsumer_458 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_458(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Template_Default_Argument_Crash_458.h"

// ========================================================================================================
#define MUT458_OUTPUT 1

void MutatorFrontendAction_458::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMemberCallExpr>("Methods")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto callee = MT->getCallee();
      if (callee == nullptr)
        return;
      auto callee_type = callee->getType();
      if (callee_type.isNull())
        return;
      auto callee_type_ptr = callee_type.getTypePtr();
      if (callee_type_ptr == nullptr)
        return;
      if (callee_type_ptr->isFunctionPointerType() == false)
        return;
      auto args = MT->getArgs();
      if (args.size() != 1)
        return;
      auto arg = args[0];
      if (arg == nullptr)
        return;
      auto arg_type = arg->getType();
      if (arg_type.isNull())
        return;
      auto arg_type_ptr = arg_type.getTypePtr();
      if (arg_type_ptr == nullptr)
        return;
      if (arg_type_ptr->isTemplateTypeParmType() == false)
        return;
      //Perform mutation on the source code text by applying string replacement
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          MT->getSourceRange());
      llvm::outs() << text << '\n';
      text = stringutils::replace(text, "<", "/*mut458*/<");
      text = stringutils::replace(text, "(", "<");
      text = stringutils::replace(text, ")", ">");
      llvm::outs() << text << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getSourceRange(), text);
    }
}
  
void MutatorFrontendAction_458::MutatorASTConsumer_458::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMemberCallExpr().bind("Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}