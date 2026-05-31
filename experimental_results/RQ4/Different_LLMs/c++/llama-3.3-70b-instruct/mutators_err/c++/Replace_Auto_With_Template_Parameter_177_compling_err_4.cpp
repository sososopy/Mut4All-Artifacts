//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/AST.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/SourceMgr.h"

/**
 * Replace_Auto_With_Template_Parameter_177
 */ 
class MutatorFrontendAction_177 : public MutatorFrontendAction {
public:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_177>(TheRewriter);
    }

private:
    class MutatorASTConsumer_177 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_177(Rewriter &R) : TheRewriter(R) {}
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
    Rewriter TheRewriter;
};

//source file
#include "../include/Replace_Auto_With_Template_Parameter_177.h"

// ========================================================================================================
#define MUT177_OUTPUT 1

void MutatorFrontendAction_177::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto range = MT->getSourceRange();
      auto begin = range.getBegin();
      auto end = range.getEnd();
      std::string declaration = llvm::StringRef(Result.SourceManager->getBuffer(begin)->getBuffer()->getBufferStart() + begin.getOffset(),
                                                 end.getOffset() - begin.getOffset()).str();
      declaration = Result.SourceManager->getBuffer(begin)->getBuffer()->getBufferStart() + begin.getOffset();
      declaration = declaration.substr(0, end.getOffset() - begin.getOffset());
      //Perform mutation on the source code text by applying string replacement
      size_t pos = declaration.find("auto");
      if (pos != std::string::npos) {
        declaration.replace(pos, 4, "T");
        // Add template parameter declaration
        declaration = "template <typename T> " + declaration;
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_177::MutatorASTConsumer_177::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}