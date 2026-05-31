//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Specialization_With_Alternate_Partial_Specialization_147
 */ 
class MutatorFrontendAction_147 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(147)

private:
    class MutatorASTConsumer_147 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_147(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
        std::vector<const clang::ClassTemplatePartialSpecializationDecl *> partialSpecs;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Mutator_Replace_Template_Specialization_With_Alternate_Partial_Specialization_147.h"

// ========================================================================================================
#define MUT147_OUTPUT 1

void MutatorFrontendAction_147::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *PS = Result.Nodes.getNodeAs<clang::ClassTemplatePartialSpecializationDecl>("PartialSpec")) {
      //Filter nodes in header files
      if (!PS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     PS->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      partialSpecs.push_back(PS);
    }
    else if (auto *PS = Result.Nodes.getNodeAs<clang::ClassTemplatePartialSpecializationDecl>("SelectedPartialSpec")) {
      if (!PS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     PS->getLocation()))
        return;
      if (partialSpecs.empty())
        return;
      //Get the source code text of target node
      auto sourceRange = PS->getSourceRange();
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
      
      //Perform mutation on the source code text by applying string replacement
      // Find the template argument list pattern
      std::string pattern = PS->getTemplateArgsAsWritten().getAsString();
      std::string mutatedPattern = pattern;
      
      // Modify the pattern based on its type
      if (pattern.find('*') != std::string::npos && pattern.find("const") == std::string::npos) {
        // Add const qualifier after pointer
        size_t starPos = pattern.find('*');
        mutatedPattern.insert(starPos + 1, " const");
      }
      else if (pattern.find('[') != std::string::npos && pattern.find(']') != std::string::npos) {
        // Change array size
        size_t openBracket = pattern.find('[');
        size_t closeBracket = pattern.find(']');
        if (closeBracket > openBracket + 1) {
          std::string oldSize = pattern.substr(openBracket + 1, closeBracket - openBracket - 1);
          std::string newSize = "10"; // Different constant
          if (oldSize != "10") {
            mutatedPattern.replace(openBracket + 1, closeBracket - openBracket - 1, newSize);
          }
          else {
            mutatedPattern.replace(openBracket + 1, closeBracket - openBracket - 1, "5");
          }
        }
      }
      else if (pattern.find("...") != std::string::npos) {
        // Adjust template argument pack usage
        mutatedPattern = "typename... Args";
      }
      
      // Replace the original pattern with mutated pattern
      size_t patternPos = originalText.find(pattern);
      if (patternPos != std::string::npos && mutatedPattern != pattern) {
        std::string mutatedText = originalText;
        mutatedText.replace(patternPos, pattern.length(), mutatedPattern);
        mutatedText = "/*mut147*/" + mutatedText;
        
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
      }
    }
}
  
void MutatorFrontendAction_147::MutatorASTConsumer_147::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplatePartialSpecializationDecl().bind("PartialSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
    
    // Second pass to actually mutate one of the recorded partial specializations
    if (!callback.partialSpecs.empty()) {
        MatchFinder secondFinder;
        size_t idx = getrandom::getRandomIndex(callback.partialSpecs.size() - 1);
        const clang::ClassTemplatePartialSpecializationDecl* targetSpec = callback.partialSpecs[idx];
        
        // Create a matcher for the specific partial specialization we want to mutate
        DeclarationMatcher specificMatcher = classTemplatePartialSpecializationDecl(
            hasName(targetSpec->getNameAsString())
        ).bind("SelectedPartialSpec");
        
        secondFinder.addMatcher(specificMatcher, &callback);
        secondFinder.matchAST(Context);
    }
}