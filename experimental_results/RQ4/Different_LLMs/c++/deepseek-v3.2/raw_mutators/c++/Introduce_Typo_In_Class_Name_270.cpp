//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Typo_In_Class_Name_270
 */ 
class MutatorFrontendAction_270 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(270)

private:
    class MutatorASTConsumer_270 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_270(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> classNames;
        std::vector<const clang::CXXRecordDecl*> classDecls;
    };
};

//source file
#include "../include/Introduce_Typo_In_Class_Name_270.h"

// ========================================================================================================
#define MUT270_OUTPUT 1

void MutatorFrontendAction_270::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      //Filter nodes in header files
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      if (!CL->isCompleteDefinition())
        return;
      //Record the node information to be used in the mutation process
      std::string name = CL->getNameAsString();
      if (name.empty())
        return;
      classNames.push_back(name);
      classDecls.push_back(CL);
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ForwardDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if ((!FD->isStruct() && !FD->isClass()) || FD->isLambda())
        return;
      if (FD->isCompleteDefinition())
        return;
      //Get the source code text of target node
      auto sourceRange = FD->getSourceRange();
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
      //Perform mutation on the source code text by applying string replacement
      if (classNames.size() > 1) {
        int idx = getrandom::getRandomIndex(classNames.size() - 1);
        std::string typoName = classNames[idx];
        std::string mutatedText = originalText;
        size_t pos = mutatedText.find(FD->getNameAsString());
        if (pos != std::string::npos) {
          mutatedText.insert(pos + FD->getNameAsString().length(), " " + typoName);
          mutatedText = "/*mut270*/" + mutatedText;
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
        }
      }
    }
    else if (auto *DC = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
      //Filter nodes in header files
      if (!DC || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DC->getLocation()))
        return;
      if ((!DC->isStruct() && !DC->isClass()) || DC->isLambda())
        return;
      if (!DC->isCompleteDefinition())
        return;
      if (!DC->hasDefinition())
        return;
      //Get the source code text of target node
      auto sourceRange = DC->getSourceRange();
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
      //Perform mutation on the source code text by applying string replacement
      if (classNames.size() > 1) {
        int idx = getrandom::getRandomIndex(classNames.size() - 1);
        std::string typoName = classNames[idx];
        std::string mutatedText = originalText;
        size_t colonPos = mutatedText.find(':');
        if (colonPos != std::string::npos) {
          size_t insertPos = colonPos + 1;
          mutatedText.insert(insertPos, " class " + typoName);
          mutatedText = "/*mut270*/" + mutatedText;
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
        }
      }
    }
}
  
void MutatorFrontendAction_270::MutatorASTConsumer_270::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher classMatcher = cxxRecordDecl().bind("Classes");
    DeclarationMatcher forwardDeclMatcher = cxxRecordDecl(isDefinition()).bind("ForwardDecl");
    DeclarationMatcher derivedClassMatcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.addMatcher(forwardDeclMatcher, &callback);
    matchFinder.addMatcher(derivedClassMatcher, &callback);
    matchFinder.matchAST(Context);
}