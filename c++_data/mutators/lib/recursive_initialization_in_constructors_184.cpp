//source file
#include "../include/recursive_initialization_in_constructors_184.h"

// ========================================================================================================
#define MUT184_OUTPUT 1

void MutatorFrontendAction_184::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Constructors")) {
        //Filter nodes in header files
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                        CD->getLocation()))
            return;

        if (CD->isImplicit() || CD->getNumParams() > 0)
            return;

        //Get the source code text of target node
        auto classDecl = dyn_cast<CXXRecordDecl>(CD->getParent());
        if (!classDecl || !classDecl->hasDefinition())
            return;

        std::string className = classDecl->getNameAsString();
        std::string newMember = "int y = g(" + className + "());\n";

        //Perform mutation on the source code text by applying string replacement
        SourceLocation insertLoc = classDecl->getBraceRange().getBegin().getLocWithOffset(1);
        Rewrite.InsertText(insertLoc, "/*mut184*/" + newMember, true, true);
    }
}

void MutatorFrontendAction_184::MutatorASTConsumer_184::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConstructorDecl(isDefinition()).bind("Constructors");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}