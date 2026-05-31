//header file
#pragma once
#include "Mutator_base.h"

/**
 * Explicit_Specialization_Inherited_Constructor_414
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
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
        std::vector<const clang::CXXRecordDecl*> candidateClasses;
    };
};

//source file
#include "../include/Mutator_Explicit_Specialization_Inherited_Constructor_414.h"

// ========================================================================================================
#define MUT414_OUTPUT 1

void MutatorFrontendAction_414::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClassTemplate")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->isCompleteDefinition())
        return;
      if (!MT->getDescribedClassTemplate())
         return;
      //Check if derived class inherits constructor via using-declaration
      bool hasUsingConstructor = false;
      for (auto decl : MT->decls()) {
        if (auto usingDecl = dyn_cast<UsingDecl>(decl)) {
          for (auto shadow : usingDecl->shadows()) {
            if (auto constructorShadow = dyn_cast<ConstructorUsingShadowDecl>(shadow)) {
              hasUsingConstructor = true;
              break;
            }
          }
        }
      }
      if (!hasUsingConstructor)
        return;
      //Record candidate class
      candidateClasses.push_back(MT);
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("BaseClassTemplate")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->isCompleteDefinition())
        return;
      if (!MT->getDescribedClassTemplate())
        return;
      //Get the source code text of target node
      //Find matching derived class
      for (auto derivedClass : candidateClasses) {
        //Check if derived class inherits from this base
        for (auto base : derivedClass->bases()) {
          auto baseType = base.getType();
          if (baseType->isTemplateTypeParmType()) {
            auto templateType = dyn_cast<TemplateTypeParmType>(baseType);
            if (templateType && templateType->getDecl() == MT) {
              //Perform mutation: create explicit specialization
              std::string specialization;
              std::string derivedName = derivedClass->getNameAsString();
              std::string baseName = MT->getNameAsString();
              //Choose a template argument type
              std::string templateArg = "int";
              specialization = "\ntemplate<> class " + derivedName + "<" + templateArg + "> : public " + baseName + "<" + templateArg + "> {\npublic:\n";
              //Add explicit constructor
              specialization += "  " + derivedName + "<" + templateArg + ">(" + templateArg + " x) : " + baseName + "<" + templateArg + ">(x) {}\n";
              specialization += "};\n";
              specialization = "/*mut414*/" + specialization;
              //Replace the original AST node with the mutated one
              SourceLocation insertLoc = derivedClass->getEndLoc();
              Rewrite.InsertTextAfterToken(insertLoc, specialization);
              //Remove this derived class from candidates to avoid duplicate mutations
              candidateClasses.erase(std::remove(candidateClasses.begin(), candidateClasses.end(), derivedClass), candidateClasses.end());
              break;
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_414::MutatorASTConsumer_414::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher derivedMatcher = cxxRecordDecl(isDerivedFrom(cxxRecordDecl()), hasDescendant(usingDecl())).bind("DerivedClassTemplate");
    DeclarationMatcher baseMatcher = cxxRecordDecl().bind("BaseClassTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.addMatcher(baseMatcher, &callback);
    matchFinder.matchAST(Context);
}