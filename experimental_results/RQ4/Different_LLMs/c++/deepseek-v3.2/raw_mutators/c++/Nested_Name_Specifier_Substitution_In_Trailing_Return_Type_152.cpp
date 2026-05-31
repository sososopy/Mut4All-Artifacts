//header file
#pragma once
#include "Mutator_base.h"

/**
 * Nested_Name_Specifier_Substitution_In_Trailing_Return_Type_152
 */ 
class MutatorFrontendAction_152 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(152)

private:
    class MutatorASTConsumer_152 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_152(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> availableTypes;
        bool placeholderDeclared = false;
    };
};

//source file
#include "../include/Mutator_Nested_Name_Specifier_Substitution_In_Trailing_Return_Type_152.h"

// ========================================================================================================
#define MUT152_OUTPUT 1

void MutatorFrontendAction_152::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("TrailingReturnFunction")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      
      // Only process function templates with trailing return type
      if (!FD->getDescribedFunctionTemplate() || !FD->getReturnTypeSourceRange().isValid())
        return;
      
      // Get trailing return type location
      auto ReturnTypeRange = FD->getReturnTypeSourceRange();
      std::string ReturnTypeStr = stringutils::rangetoStr(*(Result.SourceManager), ReturnTypeRange);
      
      // Look for nested name specifier pattern t.X::Y
      size_t dotPos = ReturnTypeStr.find('.');
      if (dotPos == std::string::npos) return;
      
      size_t colonColonPos = ReturnTypeStr.find("::", dotPos);
      if (colonColonPos == std::string::npos) return;
      
      // Extract the qualifier between dot and ::
      size_t qualifierStart = dotPos + 1;
      size_t qualifierEnd = colonColonPos;
      std::string qualifier = ReturnTypeStr.substr(qualifierStart, qualifierEnd - qualifierStart);
      
      // Collect available types in scope
      if (availableTypes.empty()) {
        // Collect template parameters
        if (auto* FT = FD->getDescribedFunctionTemplate()) {
          for (unsigned i = 0; i < FT->getTemplateParameters()->size(); ++i) {
            auto* TP = FT->getTemplateParameters()->getParam(i);
            if (auto* TTP = dyn_cast<TemplateTypeParmDecl>(TP)) {
              availableTypes.push_back(TTP->getNameAsString());
            }
          }
        }
        
        // Collect class/struct/union types in current scope
        auto* DC = FD->getDeclContext();
        while (DC) {
          if (auto* Record = dyn_cast<RecordDecl>(DC)) {
            availableTypes.push_back(Record->getNameAsString());
          }
          DC = DC->getParent();
        }
        
        // Add some common placeholder types
        availableTypes.push_back("int");
        availableTypes.push_back("char");
        availableTypes.push_back("void");
      }
      
      // Choose replacement type
      std::string replacement;
      if (!availableTypes.empty()) {
        size_t idx = getrandom::getRandomIndex(availableTypes.size() - 1);
        replacement = availableTypes[idx];
      } else {
        // No available types, create placeholder
        replacement = "_MutatedType152";
        if (!placeholderDeclared) {
          std::string placeholderDecl = "\nstruct _MutatedType152 {};\n";
          Rewrite.InsertTextBefore(FD->getBeginLoc(), placeholderDecl);
          placeholderDeclared = true;
        }
      }
      
      // Perform mutation on the source code text by applying string replacement
      std::string mutatedReturnType = ReturnTypeStr;
      mutatedReturnType.replace(qualifierStart, qualifierEnd - qualifierStart, replacement);
      
      // Ensure template keyword is preserved if present
      size_t templatePos = mutatedReturnType.find("template", colonColonPos);
      if (templatePos != std::string::npos && templatePos < mutatedReturnType.find(">")) {
        // Keep template keyword intact
      }
      
      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ReturnTypeRange), mutatedReturnType);
    }
}
  
void MutatorFrontendAction_152::MutatorASTConsumer_152::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasTrailingReturn()).bind("TrailingReturnFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}