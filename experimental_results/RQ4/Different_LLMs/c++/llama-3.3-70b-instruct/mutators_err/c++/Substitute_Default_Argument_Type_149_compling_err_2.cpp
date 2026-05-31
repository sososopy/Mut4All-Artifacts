//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "llvm/Support/SourceMgr.h"

/**
 * Substitute_Default_Argument_Type_149
 */ 
class MutatorFrontendAction_149 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(149)

private:
    class MutatorASTConsumer_149 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_149(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Substitute_Default_Argument_Type_149.h"
#include "clang/ASTMatchers/ASTMatchers.h"

// ========================================================================================================
#define MUT149_OUTPUT 1

void MutatorFrontendAction_149::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>(("TemplateDecl"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Find the default argument type
      std::string defaultArgType;
      for (auto param : MT->getTemplateParameters()) {
        if (param->hasDefaultArgument()) {
          defaultArgType = param->getDefaultArgument()->getSourceRange().getBegin().printToString(*Result.SourceManager);
        }
      }
      if (!defaultArgType.empty()) {
        // Replace the default argument type with a different type
        std::string newType = "int"; // Replace with a different type
        size_t pos = declaration.find(defaultArgType);
        if (pos != std::string::npos) {
          declaration.replace(pos, defaultArgType.length(), newType);
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_149::MutatorASTConsumer_149::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = clang::ast_matchers::templateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}