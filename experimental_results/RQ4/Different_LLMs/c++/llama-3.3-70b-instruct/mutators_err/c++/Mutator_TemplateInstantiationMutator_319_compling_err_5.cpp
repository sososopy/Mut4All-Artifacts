//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

/**
 * TemplateInstantiationMutator_319
 */ 
class MutatorFrontendAction_TemplateInstantiationMutator_319 : public MutatorFrontendAction {
public:
    MutatorFrontendAction_TemplateInstantiationMutator_319() {}

private:
    class MutatorASTConsumer_TemplateInstantiationMutator_319 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_TemplateInstantiationMutator_319(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_TemplateInstantiationMutator_319.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "llvm/Support/SourceMgr.h"

// ========================================================================================================
#define MUTTEMPLATEINSTANTIATIONMUTATOR_319_OUTPUT 1

void MutatorFrontendAction_TemplateInstantiationMutator_319::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TI = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateInstantiation")) {
      //Filter nodes in header files
      if (!TI || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TI->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = llvm::StringRef(Result.SourceManager->getBufferData(TI->getSourceRange().getBegin().getBufferID(), TI->getSourceRange()).str());
      //Perform mutation on the source code text by applying string replacement
      if (declaration.find("true") != std::string::npos) {
        declaration = declaration.substr(0, declaration.find("true")) + "false" + declaration.substr(declaration.find("true") + 4);
      } else if (declaration.find("false") != std::string::npos) {
        declaration = declaration.substr(0, declaration.find("false")) + "true" + declaration.substr(declaration.find("false") + 5);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TI->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_TemplateInstantiationMutator_319::MutatorASTConsumer_TemplateInstantiationMutator_319::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl(hasTemplateArgument(0, cxxBoolLiteral(true))).bind("TemplateInstantiation");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}