//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/Decl.h"
#include "clang/AST/ExceptionSpecificationType.h"

/**
 * Replace_Exception_Specification_57
 */ 
class MutatorFrontendAction_57 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(57)

private:
    class MutatorASTConsumer_57 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_57(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Replace_Exception_Specification_57.h"
#include "clang/AST/Decl.h"
#include "clang/AST/ExceptionSpecificationType.h"

// ========================================================================================================
#define MUT57_OUTPUT 1

void MutatorFrontendAction_57::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (FD->getExceptionSpecType() != clang::ExceptionSpecType::EST_None) {
        auto exceptionSpec = FD->getExceptionSpecType();
        if (exceptionSpec == clang::ExceptionSpecType::EST_Dynamic) {
          declaration.replace(declaration.find("throw"), 5, "noexcept");
        } else if (exceptionSpec == clang::ExceptionSpecType::EST_Noexcept) {
          declaration.replace(declaration.find("noexcept"), 7, "throw(std::runtime_error)");
        } else {
          declaration.replace(declaration.find("throw("), 6, "throw(std::invalid_argument, std::out_of_range)");
        }
      } else {
        declaration.insert(declaration.find(")"), " throw(std::runtime_error)");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_57::MutatorASTConsumer_57::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}