//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Syntax_Mutation_293
 */ 
class MutatorFrontendAction_293 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(293)

private:
    class MutatorASTConsumer_293 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_293(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/template_syntax_mutation_293.h"

// ========================================================================================================
#define MUT293_OUTPUT 1

void MutatorFrontendAction_293::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::DependentTemplateSpecializationType>("Template")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto DL = MT->getDecl();
      auto DL_str = DL->getNameAsString();
      auto DL_kind = DL->getKind();
      if (DL_kind == Decl::Kind::CXXRecord) {
        DL_str = DL_str + "::";
      }
      llvm::outs() << DL_str << '\n';
      auto args = MT->getArgs();
      auto args_str = "";
      for (auto arg : args) {
        auto arg_str = clang::TypeName::getFullyQualifiedName(
            arg.getArgumentType(), *(Result.Context));
        llvm::outs() << arg_str << '\n';
        args_str = args_str + arg_str + ",";
      }
      args_str.pop_back();
      llvm::outs() << args_str << '\n';
      auto replace = DL_str + " " + args_str;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getBeginLoc(), replace);
    }
}
  
void MutatorFrontendAction_293::MutatorASTConsumer_293::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = dependentTemplateSpecializationType().bind("Template");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}