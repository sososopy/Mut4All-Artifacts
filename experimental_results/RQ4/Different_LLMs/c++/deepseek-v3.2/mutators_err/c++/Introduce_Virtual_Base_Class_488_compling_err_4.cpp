//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Virtual_Base_Class_488
 */ 
class MutatorFrontendAction_488 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(488)

private:
    class MutatorASTConsumer_488 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_488(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const CXXRecordDecl*> candidateDerivedClasses;
    };
};

//source file
#include "../include/Mutator_488.h"

// ========================================================================================================
#define MUT488_OUTPUT 1

void MutatorFrontendAction_488::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->isCompleteDefinition())
        return;
      //Check if class has at least one base class
      if (MT->getNumBases() == 0)
        return;
      //Record candidate derived classes for later processing
      candidateDerivedClasses.push_back(MT);
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ProcessClass")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->isCompleteDefinition())
        return;
      if (MT->getNumBases() == 0)
        return;
      
      //Select a random candidate derived class to mutate
      if (candidateDerivedClasses.empty())
        return;
      size_t idx = getrandom::getRandomIndex(candidateDerivedClasses.size() - 1);
      const CXXRecordDecl* targetClass = candidateDerivedClasses[idx];
      
      //Ensure we're processing the same class we matched earlier
      if (targetClass != MT)
        return;
      
      //Get the source code text of target node
      SourceRange classRange = targetClass->getSourceRange();
      std::string classText = stringutils::rangetoStr(*(Result.SourceManager), classRange);
      
      //Select a random base class to make virtual
      int baseCount = targetClass->getNumBases();
      int baseIdx = getrandom::getRandomIndex(baseCount - 1);
      const CXXBaseSpecifier &baseSpec = *(targetClass->bases_begin() + baseIdx);
      
      //Check if base is already virtual
      if (baseSpec.isVirtual())
        return;
      
      //Get base class type and access specifier
      QualType baseType = baseSpec.getType();
      AccessSpecifier access = baseSpec.getAccessSpecifier();
      std::string accessStr;
      switch(access) {
        case AS_public: accessStr = "public"; break;
        case AS_protected: accessStr = "protected"; break;
        case AS_private: accessStr = "private"; break;
        default: accessStr = "public"; break;
      }
      
      //Find the inheritance specifier in the source text
      std::string baseTypeStr = baseType.getAsString();
      std::string searchStr = accessStr + " " + baseTypeStr;
      size_t pos = classText.find(searchStr);
      if (pos == std::string::npos) {
        //Try without access specifier (for structs)
        searchStr = baseTypeStr;
        pos = classText.find(searchStr);
        if (pos == std::string::npos)
          return;
        accessStr = ""; //struct inheritance is public by default
      }
      
      //Perform mutation on the source code text by applying string replacement
      std::string newInheritance = "virtual " + accessStr + " " + baseTypeStr;
      classText.replace(pos, searchStr.length(), newInheritance);
      
      //Now adjust constructors to initialize virtual base
      //Find all constructors of this class
      for (auto* decl : targetClass->decls()) {
        if (auto* ctor = dyn_cast<CXXConstructorDecl>(decl)) {
          if (!ctor->isImplicit() && ctor->hasBody()) {
            //Check if constructor already initializes this base
            bool alreadyInitializes = false;
            for (auto init : ctor->inits()) {
              if (init->isBaseInitializer()) {
                QualType baseClassType = init->getBaseClass()->getType();
                if (baseClassType.getAsString() == baseTypeStr) {
                  alreadyInitializes = true;
                  break;
                }
              }
            }
            
            if (!alreadyInitializes) {
              //Need to add initializer for virtual base
              SourceRange ctorRange = ctor->getSourceRange();
              std::string ctorText = stringutils::rangetoStr(*(Result.SourceManager), ctorRange);
              
              //Find member initializer list
              size_t colonPos = ctorText.find(':');
              if (colonPos != std::string::npos) {
                //Insert virtual base initializer at beginning of initializer list
                std::string initInsert = baseTypeStr + "()";
                //Check if base has default constructor (simplified assumption)
                ctorText.insert(colonPos + 1, " " + initInsert + ", ");
              } else {
                //No initializer list, need to add one
                size_t paramEnd = ctorText.find(')');
                if (paramEnd != std::string::npos) {
                  ctorText.insert(paramEnd + 1, " : " + baseTypeStr + "()");
                }
              }
              //Replace constructor text
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(ctorRange), ctorText);
            }
          }
        }
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(classRange), classText);
    }
}
  
void MutatorFrontendAction_488::MutatorASTConsumer_488::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("DerivedClass");
    DeclarationMatcher processMatcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("ProcessClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(processMatcher, &callback);
    matchFinder.matchAST(Context);
}