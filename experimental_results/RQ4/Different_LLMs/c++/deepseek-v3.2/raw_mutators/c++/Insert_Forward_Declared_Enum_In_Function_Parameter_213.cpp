//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Forward_Declared_Enum_In_Function_Parameter_213
 */ 
class MutatorFrontendAction_213 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(213)

private:
    class MutatorASTConsumer_213 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_213(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> generatedEnumNames;
    };
};

//source file
#include "../include/Mutator_Insert_Forward_Declared_Enum_In_Function_Parameter_213.h"

// ========================================================================================================
#define MUT213_OUTPUT 1

void MutatorFrontendAction_213::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("MethodDecl")) {
      //Filter nodes in header files
      if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MD->getLocation()))
        return;
      //Get the source code text of target node
      auto paramCount = MD->getNumParams();
      if (paramCount == 0) return;
      
      //Generate unique enum name
      std::string enumName = "MutEnum_213";
      int suffix = 0;
      while (std::find(generatedEnumNames.begin(), generatedEnumNames.end(), enumName + std::to_string(suffix)) != generatedEnumNames.end()) {
          suffix++;
      }
      enumName = enumName + std::to_string(suffix);
      generatedEnumNames.push_back(enumName);
      
      //Select random parameter position
      int insertPos = getrandom::getRandomIndex(paramCount);
      
      //Get parameter list source range
      auto funcDecl = MD->getSourceRange();
      SourceLocation paramStart = MD->getLocation();
      //Find parameter list start
      std::string funcText = stringutils::rangetoStr(*(Result.SourceManager), funcDecl);
      size_t paramListStart = funcText.find('(');
      if (paramListStart == std::string::npos) return;
      
      //Build insertion text
      std::string insertText = "enum " + enumName + ";";
      if (insertPos == paramCount) {
          //Insert at end
          insertText = ", " + insertText;
      } else {
          //Insert before selected parameter
          insertText = insertText + ", ";
      }
      
      //Find exact insertion location
      SourceLocation insertLoc;
      if (insertPos == 0) {
          insertLoc = paramStart.getLocWithOffset(paramListStart + 1);
      } else {
          const clang::ParmVarDecl* prevParam = MD->getParamDecl(insertPos - 1);
          insertLoc = prevParam->getEndLoc();
      }
      
      //Perform mutation on the source code text by applying string replacement
      Rewrite.InsertTextAfterToken(insertLoc, insertText);
    }
}
  
void MutatorFrontendAction_213::MutatorASTConsumer_213::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl().bind("MethodDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}