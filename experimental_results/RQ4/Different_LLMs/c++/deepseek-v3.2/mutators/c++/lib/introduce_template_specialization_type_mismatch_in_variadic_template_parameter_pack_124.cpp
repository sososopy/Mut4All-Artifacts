//source file
#include "../include/Mutator_Introduce_Template_Specialization_Type_Mismatch_In_Variadic_Template_Parameter_Pack_124.h"

// ========================================================================================================
#define MUT124_OUTPUT 1

void MutatorFrontendAction_124::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("targetCall")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      //Record the node information to be used in the mutation process
      targetCalls.push_back(CE);
    }
    else if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("selectedCall")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto callText = stringutils::rangetoStr(*(Result.SourceManager), CE->getSourceRange());
      
      //Analyze arguments to find suitable template specialization argument
      auto args = CE->getArgs();
      unsigned numArgs = CE->getNumArgs();
      if (numArgs == 0) return;
      
      //Select one argument to mutate
      unsigned argIndex = getrandom::getRandomIndex(numArgs - 1);
      auto arg = args[argIndex];
      
      //Check if argument is a constructor expression with template specialization
      if (auto *CtorExpr = dyn_cast<clang::CXXConstructExpr>(arg)) {
        auto CtorDecl = CtorExpr->getConstructor();
        if (!CtorDecl) return;
        
        //Get the class type being constructed
        auto RecordType = CtorDecl->getParent();
        if (!RecordType) return;
        
        //Check if it's a template specialization
        auto TemplateSpecialization = dyn_cast<clang::ClassTemplateSpecializationDecl>(RecordType);
        if (!TemplateSpecialization) return;
        
        //Get template arguments
        const auto &TemplateArgs = TemplateSpecialization->getTemplateArgs();
        unsigned numTemplateArgs = TemplateArgs.size();
        if (numTemplateArgs < 2) return;
        
        //Look for integral type template argument that can be mutated
        for (unsigned i = 0; i < numTemplateArgs; ++i) {
          auto Arg = TemplateArgs.get(i);
          if (Arg.getKind() == clang::TemplateArgument::Type) {
            auto Type = Arg.getAsType();
            if (Type->isIntegralType(*Result.Context)) {
              //Found integral type - mutate to different integral type
              std::string newType;
              if (Type->isSignedIntegerType()) {
                //Choose different signed integral type
                int choice = getrandom::getRandomIndex(2);
                if (choice == 0) newType = "short";
                else if (choice == 1) newType = "long";
                else newType = "long long";
              } else {
                //Choose different unsigned integral type
                int choice = getrandom::getRandomIndex(2);
                if (choice == 0) newType = "unsigned short";
                else if (choice == 1) newType = "unsigned long";
                else newType = "unsigned long long";
              }
              
              //Get the full argument text
              auto argText = stringutils::rangetoStr(*(Result.SourceManager), arg->getSourceRange());
              
              //Find the template specialization type in the argument text
              auto RecordName = RecordType->getNameAsString();
              size_t templateStart = argText.find(RecordName + "<");
              if (templateStart == std::string::npos) return;
              
              size_t typeStart = templateStart + RecordName.length() + 1;
              size_t typeEnd = argText.find(',', typeStart);
              if (typeEnd == std::string::npos) {
                //Maybe only one template argument before the size parameter
                typeEnd = argText.find(',', typeStart);
                if (typeEnd == std::string::npos) {
                  typeEnd = argText.find('>', typeStart);
                  if (typeEnd == std::string::npos) return;
                }
              }
              
              //Replace the type
              std::string mutatedArgText = argText;
              mutatedArgText.replace(typeStart, typeEnd - typeStart, newType);
              
              //Replace in the full call text
              size_t argPosInCall = 0;
              //Find position of this argument in the call text
              std::string beforeArg;
              for (unsigned j = 0; j < argIndex; ++j) {
                auto prevArgText = stringutils::rangetoStr(*(Result.SourceManager), args[j]->getSourceRange());
                beforeArg += prevArgText;
                if (j < argIndex - 1) beforeArg += ", ";
              }
              argPosInCall = beforeArg.length();
              if (argIndex > 0) argPosInCall += 2; // Account for ", "
              
              std::string mutatedCallText = callText;
              mutatedCallText.replace(argPosInCall, argText.length(), mutatedArgText);
              
              mutatedCallText = "/*mut124*/" + mutatedCallText;
              
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), mutatedCallText);
              return;
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_124::MutatorASTConsumer_124::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    //First find all candidate calls
    StatementMatcher callMat1cher = callExpr().bind("targetCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(callMat1cher, &callback);
    matchFinder.matchAST(Context);
    
    //Now select one of the recorded calls to mutate
    if (callback.targetCalls.empty()) return;
    
    unsigned selectedIndex = getrandom::getRandomIndex(callback.targetCalls.size() - 1);
    auto selectedCall = callback.targetCalls[selectedIndex];
    
    //Create a new match finder for the selected call
    MatchFinder selectedFinder;
    auto selectedMatcher = callExpr(hasDescendant(expr().bind("selectedCall"))).bind("selectedCall");
    selectedFinder.addMatcher(selectedMatcher, &callback);
    selectedFinder.matchAST(Context);
}