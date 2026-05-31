//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Inheritance_Hierarchy_423
 */ 
class MutatorFrontendAction_423 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(423)

private:
    class MutatorASTConsumer_423 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_423(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Inheritance_Hierarchy_423.h"

// ========================================================================================================
#define MUT423_OUTPUT 1

void MutatorFrontendAction_423::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("InheritedClasses")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto bases = MT->bases();
      if (bases.size() > 0) {
        //Perform mutation on the source code text by applying string replacement
        for (auto base : bases) {
          string baseName = base.getType().getAsString();
          if (base.getAccessSpecifier() == AccessSpecifier::AS_public) {
            //Change public inheritance to private
            string replacement = "class " + MT->getNameAsString() + ": private " + baseName + " {";
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), replacement);
          } else if (base.getAccessSpecifier() == AccessSpecifier::AS_private) {
            //Change private inheritance to protected
            string replacement = "class " + MT->getNameAsString() + ": protected " + baseName + " {";
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), replacement);
          } else if (base.getAccessSpecifier() == AccessSpecifier::AS_protected) {
            //Change protected inheritance to public
            string replacement = "class " + MT->getNameAsString() + ": public " + baseName + " {";
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), replacement);
          }
        }
      }
    }
}

void MutatorFrontendAction_423::MutatorASTConsumer_423::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(hasAnyBase()).bind("InheritedClasses");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}