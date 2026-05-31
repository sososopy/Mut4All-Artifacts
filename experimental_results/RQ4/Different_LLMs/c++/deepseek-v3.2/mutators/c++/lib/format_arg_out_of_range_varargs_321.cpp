//source file
#include "../include/Mutator_321.h"

// ========================================================================================================
#define MUT321_OUTPUT 1

void MutatorFrontendAction_321::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("VarargsFunc")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Check if function is variadic
      if (!FD->isVariadic())
        return;
      
      //Get the source code text of target node
      SourceRange range = FD->getSourceRange();
      std::string funcText = stringutils::rangetoStr(*(Result.SourceManager), range);
      
      //Count named parameters (excluding variadic part)
      int numNamedParams = FD->getNumParams();
      
      //Determine invalid index
      int invalidIndex;
      if (numNamedParams == 0) {
        invalidIndex = 1; // Any positive index is invalid when there are zero named parameters
      } else {
        invalidIndex = numNamedParams + 1; // Use N+1 as invalid index
      }
      
      //Try to find existing integer constant for invalid index
      std::string indexExpr;
      bool useExistingConstant = false;
      for (const auto& constant : integerConstants) {
        //Simple check: if constant name appears in function text, use it
        if (funcText.find(constant) != std::string::npos) {
          indexExpr = constant;
          useExistingConstant = true;
          break;
        }
      }
      
      if (!useExistingConstant) {
        //Create new integer constant expression
        indexExpr = std::to_string(invalidIndex);
      }
      
      //Check if function already has format_arg attribute
      bool hasFormatArg = false;
      std::string attributeText;
      for (const auto* attr : FD->getAttrs()) {
        if (attr->getKind() == clang::attr::FormatArg) {
          hasFormatArg = true;
          //Get the attribute source text
          SourceRange attrRange = attr->getRange();
          attributeText = stringutils::rangetoStr(*(Result.SourceManager), attrRange);
          break;
        }
      }
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText;
      if (hasFormatArg) {
        //Replace existing format_arg attribute with invalid index
        //Find the index number in the attribute text and replace it
        size_t startPos = attributeText.find("format_arg(");
        if (startPos != std::string::npos) {
          size_t numStart = startPos + 11; // After "format_arg("
          size_t numEnd = attributeText.find(")", numStart);
          if (numEnd != std::string::npos) {
            std::string newAttribute = attributeText.substr(0, numStart) + 
                                       indexExpr + 
                                       attributeText.substr(numEnd);
            //Replace the attribute in the function text
            size_t attrPosInFunc = funcText.find(attributeText);
            mutatedText = funcText.substr(0, attrPosInFunc) + 
                          newAttribute + 
                          funcText.substr(attrPosInFunc + attributeText.length());
          } else {
            mutatedText = funcText; // Fallback
          }
        } else {
          mutatedText = funcText; // Fallback
        }
      } else {
        //Add new format_arg attribute with invalid index
        //Insert attribute after function declarator (before semicolon or before body)
        size_t insertPos;
        if (FD->hasBody()) {
          //Find the opening brace of the function body
          insertPos = funcText.find("{");
          if (insertPos != std::string::npos) {
            mutatedText = funcText.substr(0, insertPos) + 
                          " __attribute__((format_arg(" + indexExpr + "))) " + 
                          funcText.substr(insertPos);
          } else {
            mutatedText = funcText; // Fallback
          }
        } else {
          //Function declaration without body - insert before semicolon
          insertPos = funcText.find(";");
          if (insertPos != std::string::npos) {
            mutatedText = funcText.substr(0, insertPos) + 
                          " __attribute__((format_arg(" + indexExpr + ")))" + 
                          funcText.substr(insertPos);
          } else {
            mutatedText = funcText; // Fallback
          }
        }
      }
      
      //Add mutation marker
      if (mutatedText != funcText) {
        mutatedText.insert(0, "/*mut321*/");
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(range), mutatedText);
      }
    }
    
    //Also collect integer constant declarations for potential use
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("IntegerConstant")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      
      //Check if it's an integer constant
      if (VD->getType()->isIntegerType() && VD->hasGlobalStorage()) {
        std::string name = VD->getNameAsString();
        integerConstants.push_back(name);
      }
    }
}
  
void MutatorFrontendAction_321::MutatorASTConsumer_321::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    //Match variadic function declarations
    DeclarationMatcher funcMatcher = functionDecl(isVariadic()).bind("VarargsFunc");
    //Match integer constants for potential use in invalid index
    DeclarationMatcher constMatcher = varDecl(hasGlobalStorage(), hasType(isInteger())).bind("IntegerConstant");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.addMatcher(constMatcher, &callback);
    matchFinder.matchAST(Context);
}