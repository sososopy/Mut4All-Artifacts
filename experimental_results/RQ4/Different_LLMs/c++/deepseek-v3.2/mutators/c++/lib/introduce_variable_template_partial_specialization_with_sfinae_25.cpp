//source file
#include "../include/Mutator_Introduce_Variable_Template_Partial_Specialization_With_SFINAE_25.h"

// ========================================================================================================
#define MUT25_OUTPUT 1

void MutatorFrontendAction_25::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplate")) {
      //Filter nodes in header files
      if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTD->getLocation()))
        return;
      
      auto *RD = CTD->getTemplatedDecl();
      if (!RD || !RD->isCompleteDefinition())
        return;
      
      // Record class information
      className = RD->getNameAsString();
      if (className.empty()) return;
      
      // Get template parameter
      auto *TP = CTD->getTemplateParameters();
      if (!TP || TP->size() == 0) return;
      
      auto *TTP = dyn_cast<TemplateTypeParmDecl>(*TP->begin());
      if (!TTP) return;
      templateParamName = TTP->getNameAsString();
      if (templateParamName.empty()) templateParamName = "T";
      
      classEndLoc = RD->getEndLoc();
      
      // Check if primary variable template exists
      hasVarTemplate = false;
      auto &SM = *Result.SourceManager;
      auto mainFileID = SM.getMainFileID();
      
      // Simple check by looking for variable template declaration in the main file
      std::string searchStr = "constexpr bool " + varTemplateName;
      std::string fileContent = SM.getBufferData(mainFileID).str();
      if (fileContent.find(searchStr) != std::string::npos) {
        hasVarTemplate = true;
      }
      
      // Add primary variable template if not present
      if (!hasVarTemplate) {
        std::string primaryTemplate = "\ntemplate<typename " + templateParamName + ", typename = void>\nconstexpr bool " + 
                                      varTemplateName + " = false;\n";
        Rewrite.InsertTextAfterToken(classEndLoc, primaryTemplate);
      }
      
      // Add member function template with SFINAE
      std::string memberFunc = "\n  public:\n  template<typename U, typename = typename std::enable_if<" + 
                               varTemplateName + "<U>>::type>\n  explicit " + className + "(U) {}\n";
      
      // Add friend function declaration
      std::string friendFunc = "\n  friend auto foo(" + className + " " + templateParamName + ") { return 0; }\n";
      
      // Insert both at the end of class body (before the closing brace)
      SourceLocation insertLoc = classEndLoc;
      Rewrite.InsertTextBefore(insertLoc, friendFunc);
      Rewrite.InsertTextBefore(insertLoc, memberFunc);
      
      // Add partial specialization after class
      std::string partialSpec = "\ntemplate<typename " + templateParamName + ">\nconstexpr bool " + 
                                varTemplateName + "<" + templateParamName + ", decltype(foo(" + templateParamName + "()))> = false;\n";
      
      // Add struct that triggers instantiation
      std::string triggerStruct = "\nstruct MutatorTrigger_25 {\n  " + className + "<int> member;\n};\n";
      
      Rewrite.InsertTextAfterToken(classEndLoc, partialSpec + triggerStruct);
    }
}
  
void MutatorFrontendAction_25::MutatorASTConsumer_25::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the2 AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("ClassTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}