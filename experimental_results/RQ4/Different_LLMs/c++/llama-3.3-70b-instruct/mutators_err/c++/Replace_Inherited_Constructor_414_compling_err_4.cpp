//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Inherited_Constructor_414
 */ 
class MutatorFrontendAction_414 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(414)

private:
    class MutatorASTConsumer_414 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_414(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Inherited_Constructor_414.h"

// ========================================================================================================
#define MUT414_OUTPUT 1

void MutatorFrontendAction_414::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CST = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("CST")) {
      //Filter nodes in header files
      if (!CST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CST->getLocation()))
        return;
      // Check if the constructor is an inherited constructor
      if (CST->isImplicit() && CST->getInheritedConstructor()) {
        // Get the base class and its constructor
        auto Base = CST->getParent();
        auto BaseCST = CST->getInheritedConstructor();
        // Perform mutation on the source code text by applying string replacement
        std::string MutatedCode = "/*mut414*/" + CST->getNameAsString() + "(";
        for (auto Param : BaseCST->parameters()) {
          MutatedCode += Param->getType().getAsString() + " " + Param->getNameAsString() + ", ";
        }
        MutatedCode.pop_back();
        MutatedCode.pop_back();
        MutatedCode += ") : " + Base->getNameAsString() + "(";
        for (auto Param : BaseCST->parameters()) {
          MutatedCode += Param->getNameAsString() + ", ";
        }
        MutatedCode.pop_back();
        MutatedCode.pop_back();
        MutatedCode += ") {}";
        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(CST->getSourceRange()), MutatedCode);
      }
    }
    else if (auto *UC = Result.Nodes.getNodeAs<clang::UsingDecl>("UC")) {
      //Filter nodes in header files
      if (!UC || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UC->getLocation()))
        return;
      // Check if the using declaration is for an inherited constructor
      if (UC->getNamedDecl()->getKind() == clang::Decl::CXXConstructor) {
        // Get the base class and its constructor
        auto Base = UC->getNamedDecl()->getParent();
        auto CST = UC->getNamedDecl();
        // Perform mutation on the source code text by applying string replacement
        if (CST) {
            std::string MutatedCode = "/*mut414*/" + CST->getNameAsString() + "(";
            for (auto Param : CST->asCXXConstructorDecl()->parameters()) {
              MutatedCode += Param->getType().getAsString() + " " + Param->getNameAsString() + ", ";
            }
            MutatedCode.pop_back();
            MutatedCode.pop_back();
            MutatedCode += ") : " + Base->getNameAsString() + "(";
            for (auto Param : CST->asCXXConstructorDecl()->parameters()) {
              MutatedCode += Param->getNameAsString() + ", ";
            }
            MutatedCode.pop_back();
            MutatedCode.pop_back();
            MutatedCode += ") {}";
            // Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(UC->getSourceRange()), MutatedCode);
        }
      }
    }
}
  
void MutatorFrontendAction_414::MutatorASTConsumer_414::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher1 = cxxConstructorDecl().bind("CST");
    DeclarationMatcher matcher2 = usingDecl().bind("UC");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}