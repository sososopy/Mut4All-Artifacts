//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Lambda_Expression_Parameter_Type_386
 */ 
class MutatorFrontendAction_Replace_Lambda_Expression_Parameter_Type_386 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(Replace_Lambda_Expression_Parameter_Type_386)

private:
    class MutatorASTConsumer_Replace_Lambda_Expression_Parameter_Type_386 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Replace_Lambda_Expression_Parameter_Type_386(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Replace_Lambda_Expression_Parameter_Type_386.h"
#include "clang/AST/AST.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/SourceMgr.h"

// ========================================================================================================
#define REPLACE_LAMBDA_EXPRESSION_PARAMETER_TYPE_386_OUTPUT 1

void MutatorFrontendAction_Replace_Lambda_Expression_Parameter_Type_386::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isInMainFile(MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = llvm::StringRef(MT->getSourceRange().getBegin().printToString(*Result.SourceManager));
      //Perform mutation on the source code text by applying string replacement
      std::string parameterType = "int&";
      size_t pos = std::string(declaration).find("this auto& self");
      if (pos != std::string::npos) {
        declaration = llvm::StringRef(std::string(declaration).substr(0, pos) + parameterType + std::string(declaration).substr(pos + 13));
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getSourceRange(), declaration);
    }
}
  
void MutatorFrontendAction_Replace_Lambda_Expression_Parameter_Type_386::MutatorASTConsumer_Replace_Lambda_Expression_Parameter_Type_386::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}