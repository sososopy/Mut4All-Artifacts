//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Template_Constraint_Overload_432
 */ 
class MutatorFrontendAction_432 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(432)

private:
    class MutatorASTConsumer_432 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_432(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl*> targetClasses;
    };
};

//source file
#include "../include/Add_Template_Constraint_Overload_432.h"

// ========================================================================================================
#define MUT432_OUTPUT 1

void MutatorFrontendAction_432::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TargetClass")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      if (!MT->isCompleteDefinition() || MT->isLambda())
        return;
      //Check if class has user-declared copy constructor
      bool hasCopyConstructor = false;
      for (auto *method : MT->methods()) {
        if (method->isCopyConstructor()) {
          hasCopyConstructor = true;
          break;
        }
      }
      if (hasCopyConstructor)
        return;
      //Record the node information to be used in the mutation process
      targetClasses.push_back(MT);
      //Perform mutation on the source code text by applying string replacement
      //Add top-level declarations before the class
      SourceLocation insertLoc = MT->getBeginLoc();
      std::string tagDecl = "\ntemplate<typename T> struct tag {};\n";
      std::string checkDecl = "template<typename T> constexpr bool check(tag<T>) { return true; }\n";
      std::string conceptDecl = "template<typename T> concept matches = check(tag<T>{});\n";
      std::string topLevelInsert = tagDecl + checkDecl + conceptDecl;
      Rewrite.InsertTextBefore(insertLoc, topLevelInsert);
      //Add templated constructor with requires clause
      std::string constructorInsert = "\ntemplate<typename U> requires matches<U> explicit " + MT->getNameAsString() + "(U value) {}\n";
      SourceLocation classEndLoc = MT->getEndLoc();
      Rewrite.InsertTextBefore(classEndLoc, constructorInsert);
      //Add overloaded check function for this class type
      std::string overloadInsert = "\nbool check(const " + MT->getNameAsString() + "&) { return true; }\n";
      Rewrite.InsertTextAfterToken(classEndLoc, overloadInsert);
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("CheckFunction")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      std::string funcName = FD->getNameAsString();
      if (funcName != "check")
        return;
      //Create a function that calls check with an object containing targetClass member
      if (targetClasses.empty())
        return;
      //Select a random target class
      size_t idx = getrandom::getRandomIndex(targetClasses.size() - 1);
      const clang::CXXRecordDecl* targetClass = targetClasses[idx];
      std::string className = targetClass->getNameAsString();
      //Create a struct containing the target class as member
      std::string structName = "Container_" + className;
      std::string structDecl = "\nstruct " + structName + " { " + className + " member; };\n";
      SourceLocation insertLoc = FD->getEndLoc();
      Rewrite.InsertTextAfterToken(insertLoc, structDecl);
      //Create a function that calls check with Container object
      std::string funcDecl = "\nvoid trigger_check() { " + structName + " c; check(c.member); }\n";
      Rewrite.InsertTextAfterToken(insertLoc, funcDecl);
    }
}
  
void MutatorFrontendAction_432::MutatorASTConsumer_432::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher classMatcher = cxxRecordDecl(unless(hasDescendant(cxxConstructorDecl(isCopyConstructor())))).bind("TargetClass");
    DeclarationMatcher funcMatcher = functionDecl().bind("CheckFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.matchAST(Context);
}