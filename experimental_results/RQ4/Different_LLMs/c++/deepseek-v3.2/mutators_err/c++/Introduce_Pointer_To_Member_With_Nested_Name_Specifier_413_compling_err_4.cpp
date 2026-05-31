//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Pointer_To_Member_With_Nested_Name_Specifier_413
 */ 
class MutatorFrontendAction_413 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(413)

private:
    class MutatorASTConsumer_413 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_413(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> classesWithNestedTypes;
    };
};

//source file
#include "../include/Introduce_Pointer_To_Member_With_Nested_Name_Specifier_413.h"

// ========================================================================================================
#define MUT413_OUTPUT 1

void MutatorFrontendAction_413::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Check if it's a template function
      if (!FD->getTemplatedKind())
        return;
      //Get the source code text of target node
      auto funcRange = FD->getSourceRange();
      if (!funcRange.isValid())
        return;
      //Find a suitable class with nested type
      const clang::CXXRecordDecl *targetClass = nullptr;
      for (const auto *classDecl : classesWithNestedTypes) {
        //Check if class has nested type accessible
        bool hasNestedType = false;
        for (auto *decl : classDecl->decls()) {
          if (auto *recordDecl = dyn_cast<clang::CXXRecordDecl>(decl)) {
            if (recordDecl->getDeclContext() == classDecl) {
              hasNestedType = true;
              break;
            }
          }
        }
        if (hasNestedType) {
          targetClass = classDecl;
          break;
        }
      }
      if (!targetClass)
        return;
      //Perform mutation on the source code text by applying string replacement
      std::string className = targetClass->getNameAsString();
      std::string nestedTypeName = "Inner"; //Default nested type name
      //Find actual nested type name
      for (auto *decl : targetClass->decls()) {
        if (auto *recordDecl = dyn_cast<clang::CXXRecordDecl>(decl)) {
          if (recordDecl->getDeclContext() == targetClass) {
            nestedTypeName = recordDecl->getNameAsString();
            break;
          }
        }
      }
      std::string insertion = "\n    typename " + className + "::" + nestedTypeName + " " + className + "::* ptr = nullptr;\n    (void)ptr;\n";
      //Insert at beginning of function body
      if (FD->hasBody()) {
        auto *body = FD->getBody();
        SourceLocation insertLoc = body->getBeginLoc().getLocWithOffset(1);
        //Replace the original AST node with the mutated one
        Rewrite.InsertText(insertLoc, insertion);
      }
    }
    else if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassDecl")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      if (!CD->isCompleteDefinition())
         return;
      //Record the node information to be used in the mutation process
      //Check if class has nested types
      bool hasNested = false;
      for (auto *decl : CD->decls()) {
        if (auto *recordDecl = dyn_cast<clang::CXXRecordDecl>(decl)) {
          if (recordDecl->getDeclContext() == CD) {
            hasNested = true;
            break;
          }
        }
      }
      if (hasNested) {
        classesWithNestedTypes.push_back(CD);
      }
    }
}
  
void MutatorFrontendAction_413::MutatorASTConsumer_413::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher funcMatcher = functionDecl(isTemplate()).bind("FunctionDecl");
    DeclarationMatcher classMatcher = cxxRecordDecl().bind("ClassDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.matchAST(Context);
}