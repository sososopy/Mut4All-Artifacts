//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mismatch_Explicit_Object_Parameter_In_Defaulted_Member_197
 */ 
class MutatorFrontendAction_197 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(197)

private:
    class MutatorASTConsumer_197 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_197(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const TypeDecl*> userDefinedTypes;
        bool dummyAdded = false;
    };
};

//source file
#include "../include/Mismatch_Explicit_Object_Parameter_In_Defaulted_Member_197.h"

// ========================================================================================================
#define MUT197_OUTPUT 1

void MutatorFrontendAction_197::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TypeDecl>("UserDefinedType")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      //Record user-defined types for later use
      userDefinedTypes.push_back(TD);
    }
    else if (auto *MD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("DefaultedMethod")) {
      //Filter nodes in header files
      if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MD->getLocation()))
        return;
      
      // Check if method has explicit object parameter
      if (!MD->hasExplicitObjectParameter())
        return;
      
      // Check if method is defaulted (or will be defaulted)
      if (!MD->isDefaulted() && !MD->isPure())
        return;
      
      // Get the class containing the method
      const CXXRecordDecl* classDecl = MD->getParent();
      if (!classDecl || !classDecl->isCompleteDefinition())
        return;
      
      // Get source ranges
      SourceManager &SM = Rewrite.getSourceMgr();
      SourceLocation classEndLoc = classDecl->getEndLoc();
      
      // Find a different type to use for explicit object parameter
      QualType newType;
      std::string newTypeName;
      
      // Look for other user-defined types
      for (const TypeDecl* typeDecl : userDefinedTypes) {
        if (typeDecl->getDeclName().getAsString() != classDecl->getNameAsString()) {
          newType = Result.Context->getTypeDeclType(typeDecl);
          newTypeName = typeDecl->getNameAsString();
          break;
        }
      }
      
      // If no other user-defined type found, create a dummy struct
      if (newType.isNull()) {
        if (!dummyAdded) {
          // Insert dummy struct before the class
          std::string dummyStruct = "\nstruct Mut197_Dummy {};\n";
          Rewrite.InsertTextBefore(classDecl->getBeginLoc(), dummyStruct);
          dummyAdded = true;
        }
        newTypeName = "Mut197_Dummy";
      }
      
      // Get method source code
      std::string methodDecl = stringutils::rangetoStr(SM, MD->getSourceRange());
      
      // Replace the explicit object parameter type
      // Find the position of "this" and the type after it
      size_t thisPos = methodDecl.find("this");
      if (thisPos == std::string::npos)
        return;
      
      // Find the end of the type (before comma or closing paren)
      size_t typeEnd = methodDecl.find(',', thisPos);
      size_t parenEnd = methodDecl.find(')', thisPos);
      if (typeEnd == std::string::npos || parenEnd < typeEnd)
        typeEnd = parenEnd;
      
      if (typeEnd == std::string::npos)
        return;
      
      // Extract the part before "this" and after the type
      std::string beforeThis = methodDecl.substr(0, thisPos + 4); // "this" + space
      std::string afterType = methodDecl.substr(typeEnd);
      
      // Construct new method declaration with mismatched type
      std::string newMethodDecl = beforeThis + " const " + newTypeName + "&" + afterType;
      
      // Create out-of-class defaulted definition with mismatched type
      std::string className = classDecl->getNameAsString();
      std::string methodName = MD->getNameAsString();
      std::string returnType = MD->getReturnType().getAsString();
      
      // Build parameter list (skip the explicit object parameter)
      std::string paramList;
      for (unsigned i = 1; i < MD->getNumParams(); ++i) {
        if (i > 1) paramList += ", ";
        paramList += MD->getParamDecl(i)->getType().getAsString();
        paramList += " ";
        paramList += MD->getParamDecl(i)->getNameAsString();
      }
      
      // Construct the out-of-class definition
      std::string outOfClassDef = "\n/*mut197*/" + returnType + " " + 
                                  className + "::" + methodName + 
                                  "(this const " + newTypeName + "&";
      if (!paramList.empty())
        outOfClassDef += ", " + paramList;
      outOfClassDef += ") = default;";
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MD->getSourceRange()), newMethodDecl);
      Rewrite.InsertTextAfterToken(classEndLoc, outOfClassDef);
    }
}
  
void MutatorFrontendAction_197::MutatorASTConsumer_197::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    // First matcher to collect user-defined types
    DeclarationMatcher typeMatcher = typeDecl(unless(isImplicit())).bind("UserDefinedType");
    // Second matcher to find defaulted methods with explicit object parameters
    DeclarationMatcher methodMatcher = cxxMethodDecl(
        hasExplicitObjectParameter(),
        anyOf(isDefaulted(), isPure()),
        unless(isImplicit())
    ).bind("DefaultedMethod");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(typeMatcher, &callback);
    matchFinder.addMatcher(methodMatcher, &callback);
    matchFinder.matchAST(Context);
}