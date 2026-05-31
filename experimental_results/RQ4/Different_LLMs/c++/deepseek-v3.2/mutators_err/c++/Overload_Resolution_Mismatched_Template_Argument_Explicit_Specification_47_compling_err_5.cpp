#pragma once
#include "Mutator_base.h"

/**
 * Overload_Resolution_Mismatched_Template_Argument_Explicit_Specification_47
 */ 
class MutatorFrontendAction_47 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(47)
private:
    class MutatorASTConsumer_47 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_47(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Mutator_47.h"

// ========================================================================================================
#define MUT47_OUTPUT 1

void MutatorFrontendAction_47::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;

      // Check if the call expression involves explicit template arguments
      const Expr* callee = CE->getCallee();
      if (!callee) return;
      
      // Look for explicit template arguments in the callee expression
      // We need to find the source location of the template arguments
      SourceLocation templateStartLoc;
      SourceLocation templateEndLoc;
      bool hasExplicitTemplateArgs = false;
      
      // Try to get the source range of the template arguments
      // This is a simplified approach; in practice we might need to traverse the AST more carefully
      SourceRange callRange = CE->getSourceRange();
      std::string callText = stringutils::rangetoStr(*Result.SourceManager, callRange);
      
      // Find template argument list in the call text
      size_t templateStart = callText.find('<');
      if (templateStart != std::string::npos) {
        size_t templateEnd = callText.find('>', templateStart);
        if (templateEnd != std::string::npos) {
          hasExplicitTemplateArgs = true;
          templateStartLoc = callRange.getBegin().getLocWithOffset(templateStart);
          templateEndLoc = callRange.getBegin().getLocWithOffset(templateEnd);
        }
      }
      
      if (!hasExplicitTemplateArgs) return;
      
      // Get the template argument text
      std::string templateArgsText = stringutils::rangetoStr(*Result.SourceManager, 
          SourceRange(templateStartLoc, templateEndLoc));
      
      // Parse template arguments (simplified: split by commas)
      std::vector<std::string> templateArgs;
      size_t start = 0;
      size_t commaPos = templateArgsText.find(',');
      while (commaPos != std::string::npos) {
        templateArgs.push_back(templateArgsText.substr(start, commaPos - start));
        start = commaPos + 1;
        commaPos = templateArgsText.find(',', start);
      }
      templateArgs.push_back(templateArgsText.substr(start));
      
      if (templateArgs.empty()) return;
      
      // Choose a random template argument to replace
      int argIndex = getrandom::getRandomIndex(templateArgs.size() - 1);
      
      // Get replacement type from availableTypes
      if (availableTypes.empty()) {
        // Fallback to basic types if no types collected
        availableTypes.push_back("int");
        availableTypes.push_back("void");
        availableTypes.push_back("char");
        availableTypes.push_back("double");
      }
      
      int typeIndex = getrandom::getRandomIndex(availableTypes.size() - 1);
      std::string replacementType = availableTypes[typeIndex];
      
      // Build new template arguments text
      std::string newTemplateArgsText;
      for (size_t i = 0; i < templateArgs.size(); ++i) {
        if (static_cast<int>(i) == argIndex) {
          newTemplateArgsText += replacementType;
        } else {
          newTemplate2ArgsText += templateArgs[i];
        }
        if (i != templateArgs.size() - 1) {
          newTemplateArgsText += ',';
        }
      }
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedCallText = callText;
      mutatedCallText.replace(templateStart, templateEnd - templateStart + 1, 
          "<" + newTemplateArgsText + ">");
      mutatedCallText = "/*mut47*/" + mutatedCallText;
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(callRange, mutatedCallText);
    }
    else if (auto *TD = Result.Nodes.getNodeAs<clang::TypedefDecl>("TypedefDecl")) {
      // Collect available types from typedef declarations
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getBeginLoc()))
        return;
      
      std::string typeName = TD->getNameAsString();
      if (!typeName.empty()) {
        availableTypes.push_back(typeName);
      }
    }
    else if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassDecl")) {
      // Collect available types from class declarations
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getBeginLoc()))
        return;
      
      std::string className = CD->getNameAsString();
      if (!className.empty()) {
        availableTypes.push_back(className);
      }
    }
    else if (auto *UD = Result.Nodes.getNodeAs<clang::UsingDecl>("UsingDecl")) {
      // Collect available types from using declarations
      if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UD->getBeginLoc()))
        return;
      
      // Try to get the type name from the using declaration
      // This is simplified; actual implementation might need more AST traversal
      std::string usingText = stringutils::rangetoStr(*Result.SourceManager, UD->getSourceRange());
      // Extract type name (heuristic: look for identifiers)
      // In practice, we should traverse the AST to get the actual type
      availableTypes.push_back("TYPE_FROM_USING"); // placeholder
    }
}
  
void MutatorFrontendAction_47::MutatorASTConsumer_47::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto callExprMatcher = callExpr().bind("CallExpr");
    auto typedefMatcher = typedefDecl().bind("TypedefDecl");
    auto classMatcher = cxxRecordDecl().bind("ClassDecl");
    auto usingMatcher = usingDecl().bind("UsingDecl");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(callExprMatcher, &callback);
    matchFinder.addMatcher(typedefMatcher, &callback);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.addMatcher(usingMatcher, &callback);
    matchFinder.matchAST(Context);
}