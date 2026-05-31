//source file
#include "../include/Template_Member_Function_Definition_Relocation_472.h"

// ========================================================================================================
#define MUT472_OUTPUT 1

void MutatorFrontendAction_472::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("ExternalMethod")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Check if it's a member of a class template defined outside the class
      const clang::CXXRecordDecl *RD = MT->getParent();
      if (!RD || !RD->getDescribedClassTemplate())
        return;
      //Check if the method is defined outside the class (has a body but not inside class)
      if (!MT->hasBody())
        return;
      //Check if the method definition is outside the class template body
      clang::SourceLocation ClassLoc = RD->getBeginLoc();
      clang::SourceLocation MethodLoc = MT->getBeginLoc();
      if (!Result.Context->getSourceManager().isBeforeInTranslationUnit(ClassLoc, MethodLoc))
        return;
      //Record candidate method for relocation
      candidateMethods.push_back(MT);
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("InternalMethod")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Check if it's a member of a class template defined inside the class
      const clang::CXXRecordDecl *RD = MT->getParent();
      if (!RD || !RD->getDescribedClassTemplate())
        return;
      //Check if the method is defined inside the class (has a body inside class)
      if (!MT->hasBody())
        return;
      //Check if the method definition is inside the class template body
      clang::SourceLocation ClassLoc = RD->getBeginLoc();
      clang::SourceLocation MethodLoc = MT->getBeginLoc();
      if (!Result.Context->getSourceManager().isBeforeInTranslationUnit(ClassLoc, MethodLoc))
        return;
      //Get the source code text of target node
      std::string MethodBody = stringutils::rangetoStr(*(Result.SourceManager), MT->getBody()->getSourceRange());
      //Remove the surrounding braces
      if (MethodBody.size() >= 2 && MethodBody[0] == '{' && MethodBody[MethodBody.size()-1] == '}')
        MethodBody = MethodBody.substr(1, MethodBody.size()-2);
      //Get the class template declaration
      std::string ClassDecl = stringutils::rangetoStr(*(Result.SourceManager), RD->getSourceRange());
      //Find the position of the class closing brace
      size_t ClassEndPos = ClassDecl.rfind('}');
      if (ClassEndPos == std::string::npos)
        return;
      //Construct the external definition
      std::string TemplateParams = RD->getDescribedClassTemplate()->getTemplateParameters()->getSourceRange().isValid() ?
                                   stringutils::rangetoStr(*(Result.SourceManager), RD->getDescribedClassTemplate()->getTemplateParameters()->getSourceRange()) : "";
      std::string ClassName = RD->getNameAsString();
      std::string MethodName = MT->getNameAsString();
      std::string ReturnType = MT->getReturnType().getAsString();
      std::string Params = "";
      for (unsigned i = 0; i < MT->getNumParams(); ++i) {
        if (i > 0) Params += ", ";
        Params += MT->getParamDecl(i)->getType().getAsString() + " " + MT->getParamDecl(i)->getNameAsString();
      }
      std::string ExternalDef = "template" + TemplateParams + "\n" + ReturnType + " " + ClassName + "::" + MethodName + "(" + Params + ") {\n" + MethodBody + "\n}\n";
      //Remove the internal definition from the class
      std::string MethodDecl = stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
      size_t MethodPos = ClassDecl.find(MethodDecl);
      if (MethodPos == std::string::npos)
        return;
      ClassDecl.erase(MethodPos, MethodDecl.length());
      //Insert the external definition after the class
      clang::SourceLocation InsertLoc = RD->getEndLoc();
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(RD->getSourceRange()), ClassDecl);
      Rewrite.InsertTextAfterToken(InsertLoc, "\n/*mut472*/" + ExternalDef);
    }
}
  
void MutatorFrontendAction_472::MutatorASTConsumer_472::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher ExternalMatcher = cxxMethodDecl(isDefinition(), hasParent(cxxRecordDecl(hasDescendant(classTemplateDecl())))).bind("ExternalMethod");
    DeclarationMatcher InternalMatcher = cxxMethodDecl(isDefinition(), hasParent(cxxRecordDecl(hasDescendant(classTemplateDecl())))).bind("InternalMethod");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(ExternalMatcher, &callback);
    matchFinder.addMatcher(InternalMatcher, &callback);
    matchFinder.matchAST(Context);
}