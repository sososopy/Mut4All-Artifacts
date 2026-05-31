//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Static_Deduced_Type_429
 */ 
class MutatorFrontendAction_429 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(429)

private:
    class MutatorASTConsumer_429 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_429(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_429.h"

// ========================================================================================================
#define MUT429_OUTPUT 1

void MutatorFrontendAction_429::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("staticMember")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      
      // Check if it's a static data member in a class template
      auto *RD = dyn_cast<CXXRecordDecl>(VD->getDeclContext());
      if (!RD || !RD->getDescribedClassTemplate())
        return;
      
      // Check if type is deduced (auto or decltype)
      auto QT = VD->getType();
      if (!QT->isDependentType() && !QT.getTypePtr()->isAutoType() && !QT.getTypePtr()->isDecltypeType())
        return;
      
      // Get the source code text of target node
      auto sourceRange = VD->getSourceRange();
      if (sourceRange.isInvalid())
        return;
      
      std::string originalText = stringutils::rangetoStr(*Result.SourceManager, sourceRange);
      
      // Find template parameter
      auto *CTD = RD->getDescribedClassTemplate();
      if (!CTD || CTD->getTemplateParameters()->size() == 0)
        return;
      
      auto *TP = CTD->getTemplateParameters()->getParam(0);
      std::string templateParamName = TP->getNameAsString();
      
      // Check for nested type availability
      bool hasNestedType = false;
      std::string nestedTypeName;
      for (auto *Decl : RD->decls()) {
        if (auto *TD = dyn_cast<TypeDecl>(Decl)) {
          if (TD->getAccess() == AS_public || TD->getAccess() == AS_protected) {
            nestedTypeName = TD->getNameAsString();
            hasNestedType = true;
            break;
          }
        }
      }
      
      // Perform mutation on the source code text by applying string replacement
      std::string mutatedText;
      size_t typePos = originalText.find("static ");
      if (typePos == std::string::npos)
        return;
      
      size_t typeEnd = originalText.find(" ", typePos + 7);
      if (typeEnd == std::string::npos)
        return;
      
      std::string beforeType = originalText.substr(0, typePos + 7);
      std::string afterType = originalText.substr(typeEnd);
      
      if (QT.getTypePtr()->isAutoType()) {
        if (hasNestedType) {
          mutatedText = beforeType + "typename " + RD->getNameAsString() + "<" + templateParamName + ">::" + nestedTypeName + afterType;
        } else {
          mutatedText = beforeType + templateParamName + afterType;
        }
      } else if (QT.getTypePtr()->isDecltypeType()) {
        mutatedText = beforeType + templateParamName + "*" + afterType;
      } else {
        mutatedText = beforeType + "typename " + RD->getNameAsString() + "<" + templateParamName + ">::template nested_type" + afterType;
      }
      
      mutatedText = "/*mut429*/" + mutatedText;
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
    }
}
  
void MutatorFrontendAction_429::MutatorASTConsumer_429::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(isStaticDataMember(), hasParent(cxxRecordDecl(hasDescendant(classTemplateDecl())))).bind("staticMember");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}