//source file
#include "../include/Mutator_Introduce_Lambda_Deduced_This_Forward_Like_474.h"

// ========================================================================================================
#define MUT474_OUTPUT 1

void MutatorFrontendAction_474::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Record candidate variable declarations
      if (!VD->getType()->isVoidType() && VD->hasLocalStorage()) {
        candidateVars.push_back(VD);
      }
    }
    else if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("CXXRecordDecl")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      if (!CD->isCompleteDefinition() || CD->isLambda())
        return;
      //Record candidate class declarations
      candidateClasses.push_back(CD);
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("FieldDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Record candidate field declarations
      if (!FD->getType()->isVoidType()) {
        candidateMembers.push_back(FD);
      }
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      if (!FD->hasBody())
        return;
      auto bodyRange = FD->getBody()->getSourceRange();
      auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), bodyRange);
      
      //Check if <utility> is included
      bool utilityIncluded = false;
      auto &SM = Result.Context->getSourceManager();
      auto mainFileID = SM.getMainFileID();
      auto includes = SM.getIncludeLoc(mainFileID);
      if (includes.isValid()) {
        utilityIncluded = true;
      }
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutation;
      if (!candidateVars.empty()) {
        size_t varIndex = getrandom::getRandomIndex(candidateVars.size() - 1);
        auto varDecl = candidateVars[varIndex];
        std::string varName = varDecl->getNameAsString();
        std::string varType = varDecl->getType().getAsString();
        
        mutation = "\n/*mut474*/auto lambda_474 = [" + varName + "]<typename Self>(this Self&& self) { return std::forward_like<Self>(" + varName + "); };\nstd::move(lambda_474)();\n";
        
        if (!utilityIncluded) {
          //Insert #include <utility> at the beginning of the file
          SourceLocation fileStart = Result.Context->getSourceManager().getLocForStartOfFile(Result.Context->getSourceManager().getMainFileID());
          Rewrite.InsertText(fileStart, "#include <utility>\n");
        }
        
        //Insert lambda at the beginning of the function body
        Rewrite.InsertText(bodyRange.getBegin(), mutation);
      } else if (!candidateClasses.empty() && !candidateMembers.empty()) {
        size_t classIndex = getrandom::getRandomIndex(candidateClasses.size() - 1);
        size_t memberIndex = getrandom::getRandomIndex(candidateMembers.size() - 1);
        auto memberDecl = candidateMembers[memberIndex];
        std::string memberName = memberDecl->getNameAsString();
        
        mutation = "\n/*mut474*/auto lambda_474 = [*this]<typename Self>(this Self&& self) mutable { return std::forward_like<Self>(this->" + memberName + "); };\nstd::move(lambda_474)();\n";
        
        if (!utilityIncluded) {
          SourceLocation fileStart = Result.Context->getSourceManager().getLocForStartOfFile(Result.Context->getSourceManager().getMainFileID());
          Rewrite.InsertText(fileStart, "#include <utility>\n");
        }
        
        Rewrite.InsertText(bodyRange.getBegin(), mutation);
      }
    }
}
  
void MutatorFrontendAction_474::MutatorASTConsumer_474::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher varMatcher = varDecl().bind("VarDecl");
    DeclarationMatcher classMatcher = cxxRecordDecl().bind("CXXRecordDecl");
    DeclarationMatcher fieldMatcher = fieldDecl().bind("FieldDecl");
    DeclarationMatcher funcMatcher = functionDecl().bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.addMatcher(fieldMatcher, &callback);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.matchAST(Context);
}